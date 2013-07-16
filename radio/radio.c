/*
 * radio.h
 *
 *  Created on: Jun 15, 2013
 *      Author: Chris Dossman
 */

#include "radio.h";

void Radio_Init()
{

}


void Radio_Change_Freq(const unsigned char freq)
{

}


void Radio_Transmit(const char * tx_data)
{

}


void Radio_Receive(const char * rx_data)
{

}


unsigned char Radio_IOCTL(unsigned int instr)
{

}


unsigned char Radio_IOCTL_B(unsigned char instr_b)
{
	RF1AINSTR1B = instr;
	// No dummy write!
	unsigned char byte_data = RF1ADOUTB;

	return byte_data;
}
