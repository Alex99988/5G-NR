/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/* Header file generated by fdesign on Tue Nov 13 09:42:50 2012 */

#ifndef FD_nr_scope_h_
#define FD_nr_scope_h_
#include <simple_executable.h>
#include <common/utils/system.h>
#include <openairinterface5g_limits.h>
#include "LAYER2/NR_MAC_gNB/nr_mac_gNB.h"
#include "common/ran_context.h"
#include <openair1/PHY/defs_gNB.h>
#include <forms.h>
#include "PHY/defs_gNB.h"
//#include "PHY/defs_nrUE.h"
//#include "PHY/impl_defs_top.h"
#include "PHY/defs_nr_UE.h"

/* Forms and Objects */
typedef struct {
  FL_FORM   * phy_scope_gnb;
  FL_OBJECT * rxsig_t;
  FL_OBJECT * chest_f;
  FL_OBJECT * chest_t;
  FL_OBJECT * pusch_comp;
  FL_OBJECT * pucch_comp;
  FL_OBJECT * pucch_comp1;
  FL_OBJECT * pusch_llr;
  FL_OBJECT * pusch_tput;
  FL_OBJECT * button_0;
} FD_phy_scope_gnb;

typedef struct {
    FL_FORM   * phy_scope_nrue;
    FL_OBJECT * rxsig_t;
    FL_OBJECT * chest_f;
    FL_OBJECT * chest_t;
    FL_OBJECT * pbch_comp;
    FL_OBJECT * pbch_llr;
    FL_OBJECT * pdcch_comp;
    FL_OBJECT * pdcch_llr;
    FL_OBJECT * pdsch_comp;
    FL_OBJECT * pdsch_llr;
    FL_OBJECT * pdsch_comp1;
    FL_OBJECT * pdsch_llr1;
    FL_OBJECT * pdsch_tput;
    FL_OBJECT * button_0;
} FD_phy_scope_nrue;

typedef struct {
  int *argc;
  char **argv;
} scopeParms_t;

extern unsigned char scope_enb_num_ue;
FD_phy_scope_gnb * create_phy_scope_gnb( void );
FD_phy_scope_nrue * create_phy_scope_nrue( void );


void phy_scope_gNB(FD_phy_scope_gnb *form,
                   PHY_VARS_gNB *phy_vars_gnb,
		   RU_t *phy_vars_ru,
                   int UE_id);


void phy_scope_nrUE(FD_phy_scope_nrue *form,
                  PHY_VARS_NR_UE *phy_vars_ue,
                  int eNB_id,
                  int UE_id,
                  uint8_t subframe);


void startScope(scopeParms_t * p);


extern RAN_CONTEXT_t RC;
#endif
