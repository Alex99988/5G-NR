#ifndef _MY_TOOL_H_
#define _MY_TOOL_H_

// #include <pthread.h>
#include "openair1/PHY/TOOLS/tools_defs.h"
#include <openair1/PHY/impl_defs_top.h>
#include "executables/nr-uesoftmodem.h"
#include "PHY/phy_extern_nr_ue.h"
#include "PHY/INIT/phy_init.h"
#include "NR_MAC_UE/mac_proto.h"
#include "RRC/NR_UE/rrc_proto.h"
#include "SCHED_NR_UE/phy_frame_config_nr.h"
#include "SCHED_NR_UE/defs.h"
#include "PHY/NR_UE_TRANSPORT/nr_transport_proto_ue.h"
#include "executables/softmodem-common.h"
#include "LAYER2/nr_pdcp/nr_pdcp_entity.h"
#include "SCHED_NR_UE/pucch_uci_ue_nr.h"
#include "openair2/NR_UE_PHY_INTERFACE/NR_IF_Module.h"


int slot_to_freq(PHY_VARS_NR_UE *ue,int32_t* slot_input, unsigned char symbol, bool first_sym, unsigned char Ns);







#endif 