
#ifndef SAMPLE_H
#define SAMPLE_H

#include "mts_sample.h"
#include "mts_msg_sample.h"

syserr_t
mts_send(mts_q_handle_t q,
        mts_addr_t daddr,
        const mts_msg_ref_t * msg_ref);

mts_q_handle_t
mts_bind(mts_lnode_id_t lnode_id, mts_sap_t sap,
        const mts_bind_options_t * const opt, const char * const description);



#endif /* end of include guard: SAMPLE_H */



