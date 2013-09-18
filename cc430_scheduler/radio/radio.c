
#include "radio.h"


void Radio_Init (void)
{
	// Set the High-Power Mode Request Enable bit so LPM3 can be entered
	// with active radio enabled
	PMMCTL0_H = 0xA5;
	PMMCTL0_L |= PMMHPMRE_L;
	PMMCTL0_H = 0x00;

	WriteRfSettings(&rfSettings);

	WriteSinglePATable(PATABLE_VAL);
}
