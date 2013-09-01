/*
 * init.h
 *
 *  Created on: Aug 22, 2013
 *      Author: Chris Dossman
 */

#ifndef INIT_H_
#define INIT_H_

#include <msp430.h>
#include <stdlib.h>
#include "debug.h"
#include "aes.h"
#include "radio.h"
#include "hal_pmm.h"

#define  PACKET_LEN         (0x05)			// PACKET_LEN <= 61
#define  RSSI_IDX           (PACKET_LEN)    // Index of appended RSSI
#define  CRC_LQI_IDX        (PACKET_LEN+1)  // Index of appended LQI, checksum
#define  CRC_OK             (BIT7)          // CRC_OK bit
#define  PATABLE_VAL        (0x51)          // 0 dBm output

extern RF_SETTINGS rfSettings;

unsigned char packetReceived;
unsigned char packetTransmit;

unsigned char RxBuffer[PACKET_LEN+2];
unsigned char RxBufferLength = 0;
const unsigned char TxBuffer[PACKET_LEN]= {0x1A, 0x2B, 0x3C, 0x4D, 0x5E};
unsigned char Output[(PACKET_LEN + 2) * 2];
unsigned char buttonPressed = 0;
unsigned int i = 0;

unsigned char transmitting = 0;
unsigned char receiving = 0;

void Init (void);
void Board_Init (void);
void Clock_Init (void);
void RTC_Init (void);
void Timer_Init (void);
void Timer_Reset (void);
void Radio_Init (void);

void Transmit(unsigned char *buffer, unsigned char length);
void ReceiveOn(void);
void ReceiveOff(void);

void InitButtonLeds(void);
void InitRadio(void);

#endif /* INIT_H_ */
