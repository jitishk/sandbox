
#include <stdio.h>

typedef struct _flow_profile_t {
    uint8_t unit;
    uint8_t slice;
    uint8_t vdc;
    uint8_t profile_id;
    uint32_t    mon_handle;
    char        monitor_name[100];
    uint32_t    data[100];
    /* data */
} flow_profile_t;

enum {
    HASH_DB_NUM_BINS = 100
};

typedef struct _dll_elem_t {
    struct _dll_elem_t *next;
    struct _dll_elem_t *prev;
    void *payload;
} dll_elem_t;

typedef struct _dll_ds_t {
    dll_elem_t *first;
    dll_elem_t *last;
    unsigned int count;
    dll_elem_t *iterator;
    (bool)(compare_func)(void *a, void *b) compare_func;
    (bool)(print_func)(void *a, void *b) print_func;
} dll_ds_t;

void
dll_insert (dll_ds_t *ds, void *payload)
{
    dll_elem_t *elem = (dll_elem_t *)malloc(sizeof(dll_elem_t));
    memset(elem, 0, sizeof(dll_elem_t));
    elem->payload = payload;

    if (ds->last) {
        ds->last->next = elem;
        elem->prev = ds->last;
        ds->last = elem;
    } else {
        assert(ds->first == NULL);
        ds->first = ds->last = elem;
    }
}

void
dll_remove (dll_ds_t *ds, dll_elem_t *elem, void **payload)
{
    if (elem->prev) {
        elem->prev->next = elem->next;
    } else {
        // if elem has no previous, it is first in list. 
        ds->first = elem->next;
    }
    if (elem->next) {
        elem->next->prev = elem->prev;
    } else {
        // if elem has no next, it is last in list. 
        ds->last = elem->prev;
    }
    (*payload) = elem->payload;
    free(elem);
}

void
dll_get_next (dll_ds_t *ds, void **payload)
{
    void *ret_payload = NULL; 
    switch (ds->iterator)  {
        case NULL: // end
            ds->iterator = (void *)-1;
            ret_payload = NULL;
            break;
        case (void *) -1: // start
            if (ds->first) {
                ds->iterator = ds->first->next;
                ret_payload = ds->iterator->payload;
            } else { // no elements case
                ret_payload = NULL;
                ds->iterator = (void *)-1;

            }
            break;
        default:
            ret_payload = ds->iterator->payload;
            ds->iterator = ds->iterator->next;
            break;
    }
    (*payload) = ret_payload;
}

void
dll_print (dll_ds_t *ds)
{
    void *payload = NULL;
    dll_get_next(ds, &payload);
    while (payload) {
        ds->print_func(payload);
        ds_get_next(ds, &payload);
    }
}

typedef struct _hash_elem_t {
    void *payload;
    unsigned int payloadsize;
    void *key;
    unsigned short keysize;
} hash_elem_t;

typedef struct _hash_db_t {
    dll_ds_t array[HASH_DB_NUM_BINS];
    uint32_t   num_elements;
    // additional hash stats
} hash_db_t;

typedef struct _hash_key_t {
    uint8_t key;
} hash_key_t;

void hash_db_init (hash_db_t **db)
{
    hash_db_t *new_db = (hash_db_t *)malloc(sizeof(hash_db_t));
    memset(new_db, 0, sizeof(hash_db_t));
    (*db) = new_db;
}

void
hash_db_insert (hash_db_t *db, void *key, unsigned short keysize, void *payload)
{
    uint8_t hash_key = hash_db_key_gen(key, keysize);

    dll_ds_t *dll_ds = db->array[hash_key];
    dll_elem_t *node = dll_ds->first;
    while (node != NULL) {
        hash_elem_t *hash_node = (hash_elem_t *)node->payload;
        if ((keysize == elem->keysize) && (memcmp(hash_node->key, key, keysize) == 0)) {
            // Element already inserted. return
        }
        elem = elem->next;
    }
    assert(elem->payload != NULL);
    elem->next = (hash_elem_t *)malloc(sizeof(hash_elem_t));
    memset(elem->, 0, sizeof(hash_elem_t));
    elem->next->prev = elem;

    elem = elem->next;
    elem->payload = payload;
    elem->payloadsize = payloadsize;
    elem->key = (void *)malloc(keysize);
    memset(elem->key, 0, keysize);
    memcpy(elem->key, key, keysize);
    elem->keysize = keysize;
    db->count++;
}

void
hash_db_remove (hash_db_t *db, void *key, unsigned short keysize, void **payload)
{
    uint8_t hash_key = hash_db_key_gen(key, keysize);
    void *return_payload = NULL;

    hash_elem_t *elem = db->array[hash_key];
    while (elem->payload != NULL) {
        if ((keysize == elem->keysize) && (memcmp(key, elem->key, keysize)==0)) {
            // found data
            return_payload = elem->payload;
            if (elem->next) {
                elem->next->prev = elem->prev;
                
            break;
        }
        elem = elem->next;
    }
    assert(elem->payload != NULL);
    elem->next = (hash_elem_t *)malloc(sizeof(hash_elem_t));
    memset(elem->, 0, sizeof(hash_elem_t));
    elem->next->prev = elem;

    elem = elem->next;
    elem->payload = payload;
    elem->payloadsize = payloadsize;
    elem->key = (void *)malloc(keysize);
    memset(elem->key, 0, keysize);
    memcpy(elem->key, key, keysize);
    elem->keysize = keysize;
    db->count++;
}

void
hash_db_find (hash_db_t *db, hash_key_t *key, void **payload)
{
    hash_elem_t *bin = db->array[key->key];
    while (bin->next != NULL) {
        if (db->compare_fn(bin->payload, payload) == 0) {
            (*payload) = bin->payload;
            // Element already inserted. return
        }
        bin = bin->next;
    }
    assert(bin->payload != NULL);
    bin->next = (hash_elem_t *)malloc(sizeof(hash_elem_t));
    memset(bin->next, 0, sizeof(hash_elem_t));
    bin->next->prev = bin;
    bin->next->payload = payload;
    db->count++;
}

void
flow_profile_

typedef struct _flow_profile_db_t {
    hash_db_t       profiles;
    uint8_t         profile_id_bmap[256];
    flow_profile_t  ****profile;
    /* data */
} flow_profile_db_t;

int max_units = 1;
int max_slices = 1;
int max_vdc = 1;
int max_profiles = 1;

bool flow_profile_db_init (**flow_profile_db_t)
{
    flow_profile_db_t *flow_profile_db;
    db = (flow_profile_db_t *) malloc(sizeof(flow_profile_db_t));

    db->profile = (flow_profile_t ****) malloc(max_units*sizeof(flow_profile_t ***));
    for (int i=0; i<max_units; i++) {
        db->profile[i] = (flow_profile_t ***)malloc(max_slices*sizeof(flow_profile_t **));
        for (int j=0; j<max_units; j++) {
            db->profile[i][j] = (flow_profile_t **)malloc(max_vdc*sizeof(flow_profile_t*));
            for (int k=0; k<max_units; k++) {
                db->profile[i][j][k] = (flow_profile_t*)malloc(max_profiles*sizeof(flow_profile_t));
                for (int l=0; l<max_units; l++) {
                     memset(db->profile[i][j][k][l], 0, sizeof(flow_profile_t));
                }
            }
        }
    }
}


