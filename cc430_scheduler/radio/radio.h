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
#define PACKET_LEN			(0x15)	    	// PACKET_LEN <= 61
#define RSSI_IDX			(PACKET_LEN+1-1)  // Index of appended RSSI
#define CRC_LQI_IDX			(PACKET_LEN+2-1)  // Index of appended LQI, checksum
#define CRC_OK				(BIT7)          // CRC_OK bit
#define PATABLE_VAL			(0x51)          // 0 dBm output
#define VCO_CHARGE_PUMP_CAL	(0x30) 			// Mask used to disable VCO charge
											// pump calibration
#define FS_AUTOCAL			(0x30)			// Mask used to disable auto-calibration
											// in the frequency synthesizer

// Radio states returned with a status byte from s strobe command
#define RF_STATE				(0x70)
#define RF_STATE_IDLE			(0x00)
#define RF_STATE_RX				(0x10)
#define RF_STATE_TX				(0x20)
#define	RF_STATE_FSTXON			(0x30)
#define RF_STATE_CALIBRATE		(0x40)
#define RF_STATE_SETTLING		(0x50)
#define RF_STATE_RX_OVRFLOW		(0x60)
#define RF_STATE_TX_UNDERFLOW	(0x70)
#define RF_READY				(0x80)

void Radio_Init (void);
void Radio_Read_RX_FIFO(tByte * const, tByte);
void Radio_Transmit (tByte const * const, tByte);
void Radio_Receive_On (void);
void Radio_Receive_Off (void);
void Radio_Enable_RX_Interrupt (void);
void Radio_Disable_RX_Interrupt (void);

#endif /* RADIO_H_ */
