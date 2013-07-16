/*
 * radio.h
 *
 *  Created on: Jun 15, 2013
 *      Author: Chris Dossman
 */

#ifndef RADIO_H_
#define RADIO_H_

void Radio_Init();
void Radio_Change_Freq(const unsigned char);
void Radio_Transmit(const char *);
void Radio_Receive(const char *);
unsigned char Radio_IOCTL(unsigned int);
unsigned char Radio_IOCTL_B(unsigned char);

#endif /* RADIO_H_ */
