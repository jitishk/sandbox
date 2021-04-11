
#ifndef MTS_SAMPLE_H
#define MTS_SAMPLE_H

#include <stdint.h>
#include <uuid/uuid.h>

typedef uint32_t mts_lnode_id_t; 
enum {
    LNODE_NONE,
    LNODE_FIRST,
    LNODE_SUP = LNODE_FIRST,
    LNODE_LC,
    LNODE_MAX,
    LNODE_NUM = LNODE_MAX,
    LNODE_ANY
};

typedef enum {
    MTS_SAP_NFM_EXPORT = 302,
} mts_sap_t;

typedef int32_t mts_q_handle_t; 

typedef struct mts_bind_options_s
{
    uint32_t version;
    uint16_t flags;    /* MTS_OPTION_SAP_PERSISTENT or MTS_OPTION_SAP_NONE */
    int    qlen;     /* Max Queue lengths. If 0, max-queue-length is infinite*/
    int32_t uuid;     /* UUID of the process trying to bind */
    uint32_t node;  /* Virtual node to bind, default 0 */
    uint32_t buff_sum; /*The softlimit on the percentage of total buffer pool
                       529                         that the SAP can use, maximum is MTS_MAX_BUFF_SUM */
} mts_bind_options_t;

enum {
    MTS_ENUM_ZERO,
    MTS_ENUM_ONE,
    MTS_ENUM_TWO,
};

typedef enum _mts_opc_t {
    MTS_OPC_ZERO,
    MTS_OPC_ONE,
    MTS_OPC_TWO,
    MTS_OPC_THREE,
} mts_opc_t;

typedef struct _mts_addr_t {
    mts_opc_t opcode;
    void* payload;
} mts_addr_t;

typedef unsigned int syserr_t;

#endif /* end of include guard: MTS_SAMPLE_H */
