
#include <stdio.h>
#include "sample.h"

mts_q_handle_t
mts_bind(mts_lnode_id_t lnode_id, mts_sap_t sap,
        const mts_bind_options_t * const opt, const char * const description)
{
    printf("%s:%d", __FUNCTION__, __LINE__);
    printf("\t%d,%d,%d,%d, %s\n", opt->version, opt->flags, opt->qlen, opt->uuid, description);
    return 0;
}

syserr_t
mts_send(mts_q_handle_t q,
        mts_addr_t daddr,
        const mts_msg_ref_t * msg_ref)
{
    printf("%s:%d", __FUNCTION__, __LINE__);
    return 0;
}

#if 0
syserr_t
mts_send(mts_q_handle_t q,
        mts_addr_t daddr,
        const mts_msg_ref_t * msg_ref);

mts_q_handle_t
mts_bind(mts_lnode_id_t lnode_id, mts_sap_t sap,
        const mts_bind_options_t * const opt, const char * const description);

#endif
