
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define UNUSED __attribute__((unused))

/* temporary required parameters and defaults */
typedef uint32_t ofa_cerrno;
enum {
    OFA_EOK,
    OFA_EBUSY,
    OFA_EMPTY,
    OFA_MAX_NPUS=32,
    NPU_MASK_EXAMPLE = 0x00000001, /* npus 0, 16 and 18 */
    RBUFF_SIZE=10,
    NUM_MSGS=10000,
};

/* configure traces to a file for debugging */
FILE *fp;
FILE *npu_fp[OFA_MAX_NPUS];
void files_setup() {
    uint32_t npu_mask = NPU_MASK_EXAMPLE;
    fp = fopen("in.list", "w");
    while (npu_mask) {
        uint32_t npu_id = __builtin_ffs(npu_mask) - 1;
        char filename[50];
        snprintf(filename, sizeof(filename), "out-%d.list",npu_id);
        npu_fp[npu_id] = fopen(filename, "w");
        npu_mask = npu_mask ^ (0x1<<npu_id);
    }
}

#define OFA_LTRACE(id, format, ...) \
    do { \
        fprintf(npu_fp[id], format, __VA_ARGS__); \
        fflush(npu_fp[id]); \
    } while(0);

#define OFA_MAIN_LTRACE(format, ...) \
    do { \
        fprintf(fp, format, __VA_ARGS__); \
        fflush(fp); \
    } while (0);

/*! \struct _elem_t 
 *  \brief an element in queue.
 *
 *  Implements an element in the queue. It has a pointer to the next element
 *  and a void pointer to the message being enqueued.
 */
typedef struct _elem_t {
    struct _elem_t *next; /*!< pointer to next element in queue. */
    void *msg;
} elem_t;

/*! \struct _queue_t 
 *  \brief queue to hold messages.
 *
 *  A queue that uses two different pointers for access during post and get
 *  operations.  No mutex/protection is required to access the queue by the
 *  producer and consumer.
 */
typedef struct _queue_t {
    elem_t *post_ptr; /*!< pointer to post next message. */
    elem_t *get_ptr;
} queue_t;

void
queue_init (queue_t *queue)
{
    elem_t *elem = (elem_t *) malloc(sizeof(elem_t));
    memset(elem, 0, sizeof(elem_t));
    queue->post_ptr = queue->get_ptr = elem;
}

ofa_cerrno
queue_post (queue_t *queue, void *msg)
{
    elem_t *temp_ptr = queue->post_ptr; // store current post location
    queue->post_ptr->next = (elem_t *) malloc(sizeof(elem_t));  // insert new post location
    memset(queue->post_ptr->next, 0, sizeof(elem_t));           // and init to zeros
    queue->post_ptr = queue->post_ptr->next;                    // move pointer to next location

    asm volatile("mfence" ::: "memory");

    temp_ptr->msg = msg;    // now set the post location with the message passed.
                            // once set, queue_get can proceed.
                            // the memory barrier will prevent this line to be
                            // executed before all the above operations
    return OFA_EOK;
}

ofa_cerrno
queue_get (queue_t *queue, void **msg)
{
    elem_t *temp_ptr;
    if (queue->get_ptr->msg == NULL) {      // if there is no msg here, the queue is empty
        return OFA_EMPTY;                   // return as empty
    }
    (*msg) = queue->get_ptr->msg;           // store message to output variable

    temp_ptr = queue->get_ptr;              // store current get location
    queue->get_ptr = queue->get_ptr->next;  // and go to next one. (it must exist)

    // asm volatile("mfence" ::: "memory"); // is mem barrier needed here?
    
    free(temp_ptr); // free previous location. the post_ptr is guaranteed
                    // to have moved to away from here.
    return OFA_EOK;
}

typedef enum _npu_th_states {
    WAIT,
    MSG_Q,
    STATS_Q,
} npu_thread_stats_e;

/*! \struct _ofa_npu_thread_t
 *  \brief Brief struct description
 *
 *  ofa_npu_thread_t holds an NPU thread that processes messages posted in a
 *  ring buffer(msg_buff) A pair of mutex/cond provides signalling to this
 *  thread.
 *  Flags is_running and has_work prevents over-signalling and under-signalling
 *  respectively. accesses the flags are also protected by the mutex.
 */
typedef struct _ofa_npu_thread_t {
    uint32_t        id;
    pthread_t       thread;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    queue_t         msg_queue;
    queue_t         stats_queue;
    bool            is_running; /* TRUE if thread is running (not in cond-wait) */
    bool            has_work;   /* TRUE if there is still that is to be processed */
    npu_thread_state_e state;
} ofa_npu_thread_t;

static inline void
ofa_npu_thread_wait (ofa_npu_thread_t *npu_thread)
{
    pthread_mutex_lock(&npu_thread->mutex);
    /* check if any message was posted since last get. */
    while (npu_thread->has_work == false) {
        npu_thread->is_running = false; /* set flag to indicate it is no-longer
                                           running and going to sleep. */
        pthread_cond_wait(&npu_thread->cond, &npu_thread->mutex);
    }
    npu_thread->is_running = true;
    npu_thread->has_work = false;
    pthread_mutex_unlock(&npu_thread->mutex);
}

static inline void
ofa_npu_thread_signal (ofa_npu_thread_t *npu_thread)
{
#ifdef AVOID_LOCKS
    /* do nothing. */
#else
    pthread_mutex_lock(&npu_thread->mutex);
    /* if thread is running or flagged to have work, no need to signal. (prevent over signalling) */
    if ((npu_thread->is_running == false) && (npu_thread->has_work == false)) {
        pthread_cond_signal(&npu_thread->cond);
        npu_thread->has_work = true;    /* indicate there is work. i.e. makes
                                           signal persistent.  */
    }
    pthread_mutex_unlock(&npu_thread->mutex);
#endif
}

static ofa_cerrno
ofa_npu_thread_send_msg (ofa_npu_thread_t *thread, void *msg)
{
    //ofa_cerrno rc = rbuff_post(&thread->msg_buff, msg);
    ofa_cerrno rc = queue_post(&thread->msg_queue, msg);
    ofa_npu_thread_signal(thread);
    return rc;
}

static void
ofa_npu_thread_process_msg (void *msg)
{
    /* Process a message received. For now, just print and free the buffer. */
    /* printf("received: (%s) ", (char *)msg); */
    free(msg);
}

/*!
 * main procedure executed by the npu-thread. If there are no messages in the
 * buffer, then wait until its woken up.
 */
static void *
ofa_npu_thread_main (void * args)
{
    ofa_npu_thread_t *npu_thread = (ofa_npu_thread_t *) args;
    uint32_t count = 0;
    while (1) {

        switch(npu_thread->state) {
            case WAIT:
                ofa_npu_thread_wait(npu_thread);
                
                
            case MSG_QUEUE:
                void *msg = NULL;
                queue_get(npu_thread->msg_queue, &msg);
                break;
            case STATS_QUEUE:
                void *msg = NULL;
                queue_get(npu_thread->stats_queue, &msg);
                break;
            case default:
                //error
        }
        if (count > NUM_MSGS) {
            break;
        }
    }

    pthread_exit(NULL);
}

static void
ofa_npu_thread_join (ofa_npu_thread_t *npu_thread)
{
    pthread_join(npu_thread->thread, NULL);
}

ofa_cerrno
ofa_npu_thread_init (ofa_npu_thread_t *npu_thread, unsigned int npu_id)
{
    pthread_cond_init(&npu_thread->cond, NULL);
    pthread_mutex_init(&npu_thread->mutex, NULL);
    npu_thread->is_running = npu_thread->has_work = false;
    npu_thread->id = npu_id;

    rbuff_init(&npu_thread->msg_buff, RBUFF_SIZE);
    queue_init(&npu_thread->msg_queue);
    npu_thread->state = WAIT;
    pthread_create(&npu_thread->thread,
            NULL,                   /* thread attributes */
            ofa_npu_thread_main,    /* thread routine */
            npu_thread              /* pass thread struct as argument */
            );
    return OFA_EOK;
}

/*! \struct _ofa_npu_threads_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */
typedef struct _ofa_npu_threads_t {
    ofa_npu_thread_t thread[OFA_MAX_NPUS];
    uint32_t num_npus;
    uint32_t npu_mask;
} ofa_npu_threads_t;

static ofa_npu_threads_t npu_threads;

ofa_cerrno
ofa_npu_threads_init (uint32_t npu_mask)
{
    npu_threads.npu_mask = npu_mask;
    npu_threads.num_npus = __builtin_popcount(npu_mask);
    while (npu_mask) {
        uint32_t npu_id = __builtin_ffs(npu_mask) - 1;
        ofa_npu_thread_init(&npu_threads.thread[npu_id], npu_id);
        npu_mask = npu_mask ^ (0x1<<npu_id);
    }
    return OFA_EOK;
}

/*!
 * brodcast a message to all initialized npu_threads. A copy of the provided
 * message is posted to the buffer of each npu_thread. 
 */
ofa_cerrno
ofa_npu_threads_bcast (void *msg, size_t size)
{
    uint32_t npu_mask = npu_threads.npu_mask;
    while (npu_mask) {
        uint32_t npu_id = __builtin_ffs(npu_mask) - 1;
        /* create copy of the message to be sent. */
        void *msg_copy = (void *) malloc(size);
        memcpy(msg_copy, msg, size);
        OFA_MAIN_LTRACE("\nSending message (%s) to thread (%d)", (char *)msg_copy, npu_id);
        while (OFA_EBUSY ==
                ofa_npu_thread_send_msg(&npu_threads.thread[npu_id], msg_copy)) {
            usleep(100);
        }
        npu_mask = npu_mask ^ (0x1<<npu_id);
    }
    return OFA_EOK;
}

ofa_cerrno
ofa_npu_threads_join ()
{
    uint32_t npu_mask = npu_threads.npu_mask;
    while (npu_mask) {
        uint32_t npu_id = __builtin_ffs(npu_mask) - 1;
        ofa_npu_thread_join(&npu_threads.thread[npu_id]);
    }
    return OFA_EOK;
}

int main(int argc UNUSED, const char *argv[] UNUSED)
{
    char *msg = NULL;
    int i = 0;

    /* initialize few npu threads */
    ofa_npu_threads_init(NPU_MASK_EXAMPLE);
    files_setup(NPU_MASK_EXAMPLE);
    while (1) {
        /* create a string message to be sent. */
        size_t size = 50 * sizeof(char);
        i++;
        if (i == NUM_MSGS) {
            break;
        }
        msg = (char *)malloc(size);
        memset(msg, 0, size);
        snprintf(msg, size, "message %d", i);
        ofa_npu_threads_bcast((void*)msg, 50);
        free(msg);
    }

    printf ("Done: %d message sent", i-1);
    ofa_npu_threads_join();
    return 0;
}

