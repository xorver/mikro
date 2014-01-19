
#ifndef _CONST_H_
#define _CONST_H_

#include "main.h"

	extern uint8_t std_dc_luminance_nrcodes[17];
	extern uint8_t std_dc_luminance_values[12];
	extern uint8_t std_ac_luminance_nrcodes[17];
	extern uint8_t std_ac_luminance_values[162];
	extern uint8_t std_dc_chrominance_nrcodes[17];
	extern uint8_t std_dc_chrominance_values[12];
	extern uint8_t std_ac_chrominance_nrcodes[17];
	extern uint8_t std_ac_chrominance_values[162];

	/*Huffman tables*/
	extern uint16_t YDC_HT[256][2];
	extern uint16_t UVDC_HT[256][2];
	extern uint16_t YAC_HT[256][2];
	extern uint16_t UVAC_HT[256][2];

	extern int32_t YQT[64];
	extern int32_t UVQT[64];
	extern real aasf[8];
	extern uint8_t zigzag_order[64];

#endif
