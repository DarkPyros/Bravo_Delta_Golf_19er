/*------------------------------------------------------------------*-
  Radio.H (v1.00)
  ------------------------------------------------------------------

  Header file for CC430 radio module interface

-*------------------------------------------------------------------*/

#ifndef RADIO_H_
#define RADIO_H_

#include "../main.h"
#include "../RF1A/RF1A.h"
#include "../hSCH/hSCH.h"

// Radio configuration defines
#define  PACKET_LEN		(0x2B)	    	// PACKET_LEN <= 61
#define  RSSI_IDX		(PACKET_LEN+1)  // Index of appended RSSI
#define  CRC_LQI_IDX	(PACKET_LEN+2)  // Index of appended LQI, checksum
#define  CRC_OK			(BIT7)          // CRC_OK bit
#define  PATABLE_VAL	(0x51)          // 0 dBm output

void Radio_Init (void);
void Radio_Read_RX_FIFO(tByte * const, tByte);

#endif /* RADIO_H_ */
