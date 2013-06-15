/*
 * radio.h
 *
 *  Created on: Jun 15, 2013
 *      Author: Chris Dossman
 */

#ifndef RADIO_H_
#define RADIO_H_

void Radio_Init();
void Radio_Change_Channel(unsigned char);
void Radio_Transmit(const char *);
void Radio_Receive(const char *);

#endif /* RADIO_H_ */
