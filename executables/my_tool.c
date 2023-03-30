#include"my_tool.h"

static inline int32_t abs32(c16_t x)
{
    return (int32_t)(x.r) * (int32_t)(x.r) + (int32_t)(x.i) * (int32_t)(x.i);
//   return (((int64_t)((int32_t*)&x)[0])*((int64_t)((int32_t*)&x)[0]) + ((int64_t)((int32_t*)&x)[1])*((int64_t)((int32_t*)&x)[1]));
}


int slot_to_freq(PHY_VARS_NR_UE *ue,int32_t* slot_input, unsigned char symbol, bool first_sym, unsigned char Ns)
{
    //symbol：一个时隙里的第几个符号
    //Ns：时隙号
extern char  addr_rxdataF[60], addr_rxdataRB[60];

  NR_DL_FRAME_PARMS *frame_parms = &ue->frame_parms;
  NR_UE_COMMON *common_vars      = &ue->common_vars;

  AssertFatal(symbol < frame_parms->symbols_per_slot, "slot_fep: symbol must be between 0 and %d\n", frame_parms->symbols_per_slot-1);
  AssertFatal(Ns < frame_parms->slots_per_frame, "slot_fep: Ns must be between 0 and %d\n", frame_parms->slots_per_frame-1);

  unsigned int nb_prefix_samples;
  unsigned int nb_prefix_samples0;
  if (ue->is_synchronized) {
    nb_prefix_samples  = frame_parms->nb_prefix_samples;
    nb_prefix_samples0 = frame_parms->nb_prefix_samples0;
  } else {
    nb_prefix_samples  = frame_parms->nb_prefix_samples;
    nb_prefix_samples0 = frame_parms->nb_prefix_samples;
  }

  dft_size_idx_t dftsize = get_dft(frame_parms->ofdm_symbol_size);
  // This is for misalignment issues
  int32_t tmp_dft_in[8192] __attribute__ ((aligned (32)));

  unsigned int rx_offset = 0;
  if(first_sym){//如果是第0个符号，则偏移为第0个符号的循环前缀
    rx_offset = nb_prefix_samples0;
  }
  else{
    if(symbol == 0){//是13个符号后的第0个符号
        for (int idx_symb = 1; idx_symb < frame_parms->symbols_per_slot; idx_symb++)
          rx_offset += (idx_symb % (0x7 << frame_parms->numerology_index)) ? nb_prefix_samples : nb_prefix_samples0;
        rx_offset += frame_parms->ofdm_symbol_size * (frame_parms->symbols_per_slot - 1);
        rx_offset += nb_prefix_samples0;
    }
    else{
      for (int idx_symb = 1; idx_symb <= symbol; idx_symb++)
        rx_offset += (idx_symb % (0x7 << frame_parms->numerology_index)) ? nb_prefix_samples : nb_prefix_samples0;
      rx_offset += frame_parms->ofdm_symbol_size * (symbol - 1);
    }
  }

  // use OFDM symbol from within 1/8th of the CP to avoid ISI
  rx_offset -= (nb_prefix_samples / frame_parms->ofdm_offset_divisor);

rx_offset++;

//#ifdef DEBUG_FEP
  //  if (ue->frame <100)
  LOG_D(PHY,"slot_fep: slot %d, symbol %d, nb_prefix_samples %u, nb_prefix_samples0 %u, rx_offset %u energy %d\n",
  Ns, symbol, nb_prefix_samples, nb_prefix_samples0, rx_offset, dB_fixed(signal_energy(&common_vars->rxdata[0][rx_offset],frame_parms->ofdm_symbol_size)));
  //#endif

int32_t* output;
output = (int32_t *)malloc16(frame_parms->ofdm_symbol_size*sizeof(int32_t*));
  for (unsigned char aa=0; aa<frame_parms->nb_antennas_rx; aa++) {
    memset(output, 0, frame_parms->ofdm_symbol_size * sizeof(int32_t));

    int16_t *rxdata_ptr = (int16_t *)&slot_input[rx_offset];

    // if input to dft is not 256-bit aligned
    if ((rx_offset & 7) != 0) {
      memcpy((void *)&tmp_dft_in[0],
             (void *)&slot_input[rx_offset],
             frame_parms->ofdm_symbol_size * sizeof(int32_t));

      rxdata_ptr = (int16_t *)tmp_dft_in;
    }

    start_meas(&ue->rx_dft_stats);

//修改：测试rx_offset数据是否正确
//     FILE *fp2 = fopen("../../../data/UE/temp.iq", "w+");
//     c16_t *out = (c16_t *)rxdata_ptr;
//     for (int i = 0; i < frame_parms->ofdm_symbol_size; i++) {
//       // if(out[i].r != 0){
//       fwrite(&out[i].r, sizeof(out[i].r), 1, fp2);
//       fwrite(&out[i].i, sizeof(out[i].i), 1, fp2);
//       //}
//     }
//   fclose(fp2);

    dft(dftsize,
        rxdata_ptr,
        (int16_t *)&output[0],
        1);

    stop_meas(&ue->rx_dft_stats);

    c16_t rot2;
    if (first_sym) {
      int symb_offset = (Ns % frame_parms->slots_per_subframe) * frame_parms->symbols_per_slot;
      rot2 = frame_parms->symbol_rotation[0][symbol + symb_offset];
      rot2.i = -rot2.i;
    } else if (symbol != 0) {
      int symb_offset = (Ns % frame_parms->slots_per_subframe) * frame_parms->symbols_per_slot;
      rot2 = frame_parms->symbol_rotation[0][symbol + symb_offset];
      rot2.i = -rot2.i;
    } else {
      Ns++;
      int symb_offset = (Ns % frame_parms->slots_per_subframe) * frame_parms->symbols_per_slot;
      rot2 = frame_parms->symbol_rotation[0][symbol + symb_offset];
      rot2.i = -rot2.i;
    }

#ifdef DEBUG_FEP
    //  if (ue->frame <100)
    printf("slot_fep: slot %d, symbol %d rx_offset %u, rotation symbol %d %d.%d\n", Ns,symbol, rx_offset,
	   symbol+symb_offset,rot2.r,rot2.i);
#endif

    c16_t *shift_rot = frame_parms->timeshift_symbol_rotation;
    c16_t *this_symbol = (c16_t *)&output[0];

    if (frame_parms->N_RB_DL & 1) {
      rotate_cpx_vector(this_symbol, &rot2, this_symbol,
                        (frame_parms->N_RB_DL + 1) * 6, 15);
      rotate_cpx_vector(this_symbol + frame_parms->first_carrier_offset - 6,
                        &rot2,
                        this_symbol + frame_parms->first_carrier_offset - 6,
                        (frame_parms->N_RB_DL + 1) * 6, 15);
      multadd_cpx_vector((int16_t *)this_symbol, (int16_t *)shift_rot, (int16_t *)this_symbol,
                         1, (frame_parms->N_RB_DL + 1) * 6, 15);
      multadd_cpx_vector((int16_t *)(this_symbol + frame_parms->first_carrier_offset - 6),
                         (int16_t *)(shift_rot   + frame_parms->first_carrier_offset - 6),
                         (int16_t *)(this_symbol + frame_parms->first_carrier_offset - 6),
                         1, (frame_parms->N_RB_DL + 1) * 6, 15);
    } else {
      rotate_cpx_vector(this_symbol, &rot2, this_symbol,
                        frame_parms->N_RB_DL * 6, 15);
      rotate_cpx_vector(this_symbol + frame_parms->first_carrier_offset,
                        &rot2,
                        this_symbol + frame_parms->first_carrier_offset,
                        frame_parms->N_RB_DL * 6, 15);
      multadd_cpx_vector((int16_t *)this_symbol, (int16_t *)shift_rot, (int16_t *)this_symbol,
                         1, frame_parms->N_RB_DL * 6, 15);
      multadd_cpx_vector((int16_t *)(this_symbol + frame_parms->first_carrier_offset),
                         (int16_t *)(shift_rot   + frame_parms->first_carrier_offset),
                         (int16_t *)(this_symbol + frame_parms->first_carrier_offset),
                         1, frame_parms->N_RB_DL * 6, 15);
    }
  }

//写文件
c16_t *out=(c16_t *)output;   
FILE* fp1 = fopen(addr_rxdataF, "ab+");
for (int i = 0; i < frame_parms->ofdm_symbol_size; i++) {
  // if(out[i].r != 0){
  fwrite(&out[i].r, sizeof(out[i].r), 1, fp1);
  fwrite(&out[i].i, sizeof(out[i].i), 1, fp1);
  //}
}
fclose(fp1);

uint32_t       N_RB_DL    = 106;
int8_t rb_out[N_RB_DL];
for (int i = 0; i < N_RB_DL; i++) {
  rb_out[i] = 0;
  for (int j = 0; j < 12; j++) {
    int k = frame_parms->first_carrier_offset + i * 12 + j;
    if (k >= frame_parms->ofdm_symbol_size)
      k -= frame_parms->ofdm_symbol_size;
    if(abs32(out[k]) > 5000)
    {
        rb_out[i] = 1;
        break;
    }
  }
}

fp1 = fopen(addr_rxdataRB, "ab+");
for (int i = 0; i < N_RB_DL; i++) {
  fwrite(&rb_out[i], sizeof(rb_out[i]), 1, fp1);
}
fclose(fp1);

#ifdef DEBUG_FEP
  printf("slot_fep: done\n");
#endif

return 0;
}
