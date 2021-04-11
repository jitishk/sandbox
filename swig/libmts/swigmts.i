%module swigmts
%{
#include <isan/mts_intf.h>
#include <isan/im_msg.h>
#include <isan/nf_api.h>
%}

#define __attribute__(x)
#define PACKED 
typedef unsigned short  uint16;
typedef unsigned int  uint32;
typedef unsigned char  uint8;
typedef signed short  int16;
typedef signed int  int32;
typedef signed char  int8;

// TODO: include actual header
//%include <../../../global/constants/export/dcos/uuid.h>   //MTS and UUIDs for processes
typedef enum {
    SRVUUID_NFM = 405,
    SRVUUID_HW_TELEMETRY = 2620,
} uuid_t;


// TODO: handle inline functions in header
// TODO: sysmgr_vdc_get()
//%include <../../../global/export/isan/types.h> // if_ordinal
//%include <../../../global/export/isan/enum_table.h> // figure how to handle inline functions in header. (__str2enum())

// TODO: include actual header
//#include <../../../utils/libtlv/export/isan/tlv_utils.h>
#define TLVU_STRUCT_GENERIC_PROTOTYPES(a,b)

/** MTS **/
// TODO: include actual header
// %include <../../../include/isan/mts_saps.h>
typedef enum {
    MTS_SAP_ACLQOS = 190,
    MTS_SAP_NFM = 306,
    MTS_SAP_HW_TELEMETRY = 1524,
} mts_sap_t;

// TODO:  why is this not picked up from mts_defs.h
typedef int32 mts_q_handle_t;
// typedef uint32_t mts_sap_uint32;

%include <../export/isan/mts_defs.h>
%include <../export/isan/mts_intf.h>


// TODO: how to include mts_opcodes.h. It is disable for non-kernel applications.
typedef int32 mts_opc_t;

// Hack to allow int types to initialize mts_sap_t.node/sap. 
%extend mts_addr_t {
    char mts_addr_t_str[100];
    mts_addr_t(int node, int sap) {
        mts_addr_t *addr;
        addr = (mts_addr_t *) calloc(1, sizeof(mts_addr_t));
        addr->node = *((uint32 *)&node);
        addr->sap = *((mts_sap_uint32 *)&sap);
        return addr;
    }
    char * __str__() {
        mts_addr_t_str[0] = '\0';
        int n = snprintf(mts_addr_t_str, sizeof(mts_addr_t_str),
                "node: %u sap: %u", $self->node, *((uint32_t *)&$self->sap));
        mts_addr_t_str[n] = '\0';
        return mts_addr_t_str;
    }
}

/** FSM UTILS **/
//%include <../../../utils/fsmutils/export/isan/fsm_utils_intf.h>

/** NF **/
%include <../../../feature/forwarding-sw/netflow/lib/export/isan/nf_api.h>
%extend nf_msg2acl_t {
    nf_msg2acl_t(uint32 num) {
        nf_msg2acl_t *msg;
        msg = (nf_msg2acl_t *) malloc(sizeof(nf_msg2acl_t) + \
                 num*sizeof(nf_msg_acl_t));
        msg->msg_count = num;
        return msg;
    }
    nf_msg_acl_t * get_msg(int i) {
        if (i < 0 || i > $self->msg_count) {
            return NULL;
        }
        return &$self->msg[i];
    }
    ~nf_msg2acl_t() {
        free($self);
    }
}

%extend nf_msg_acl_t {
    nf_msg_acl_t *__getitem__(int i) {
        return &$self[i];
    }
}

/** Interface **/
// #define PORT_NAME_MAXLEN    81
// typedef char pm_port_name_t [PORT_NAME_MAXLEN];
// %include <../../../infra/port_manager/lib/export/isan/pm_defs.h>
// %include <../../../infra/interface_manager/lib/export/isan/im_msg.h>
// %extend im_if_created_pld_t {
//     im_if_created_pld_t(uint32 num) {
//         im_if_created_pld_t *intf;
//         intf = (im_if_created_pld_t *) malloc(sizeof(im_if_created_pld_t) + \
//                  num*sizeof(im_if_created_info_t));
//         intf->num = num;
//         return intf;
//     }
//     ~im_if_created_pld_t() {
//         free($self);
//     }
// }

//%include <../../../infra/interface_manager/lib/export/isan/im_defs.h> // for im_intf_type_t

/*
mts_opc_t
mts_addr_t
mts_send_options_t
size_t
mts_opc_get/mts_opc_set
MTS_OPC_IM_IF_CREATED
im_if_created_pld_t

*/
/*
import swigmts
mts_addr_dst = swigmts.mts_addr_t(swigmts.MTS_NODE_THIS, swigmts.MTS_SAP_ACLQOS)
k = swigmts.mts_bind_options_t()
k.version = swigmts.MTS_OPT_VERSION_1
k.qlen = 4096
k.flags = swigmts.MTS_OPTION_SAP_PERSISTENT
k.uuid = swigmts.SRVUUID_HW_TELEMETRY
k.node = 0
k.buff_sum = swigmts.MTS_MAX_BUFF_SUM
z = swigmts.mts_bind(swigmts.LNODE_SUP, swigmts.MTS_SAP_HW_TELEMETRY, k, "NFm TestD SAP")
*/

