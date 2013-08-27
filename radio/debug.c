/*
 * debug.c
 *
 *  Created on: Aug 22, 2013
 *      Author: Chris Dossman
 */

#include <msp430.h>
#include <stdlib.h>
#include "debug.h"

unsigned char BinaryText[128] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

unsigned char HexText[32] = { 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00 };

unsigned char Timer_Hex[4] = { 0x00, 0x00, 0x00, 0x00 };

void UART_Init (void)
{
   P1SEL |= BIT5 + BIT6;

   UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
   UCA0CTL0 = 0x00;                          // UART - 8/N/1 with LSB first
   UCA0CTL1 |= UCSSEL_2;                     // SMCLK
   UCA0BR0 = 0x5B;                           // 20MHz 57600 (see User's Guide)
   UCA0BR1 = 1;                              // 20MHz 57600
   UCA0MCTL |= UCBRS_2 + UCBRF_0;            // Modulation UCBRSx=2, UCBRFx=0
   UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void UART_TX (unsigned char * data, unsigned char size)
{
   unsigned int index = 0;

   for (index = size; index > 0; index--)
   {
      while (UCA0STAT & UCBUSY);   // wait for UART to be free

      UCA0TXBUF = data[index - 1];	// transmit random byte data
   }

   #if (defined TEXT_BINARY || defined TEXT_HEX)
      while (UCA0STAT & UCBUSY);   // wait for UART to be free

      UCA0TXBUF = 0x0D;	// transmit CR

      while (UCA0STAT & UCBUSY);   // wait for UART to be free

      UCA0TXBUF = 0x0A;	// transmit LF
   #endif
}

void Convert_To_ASCII_Binary (unsigned char * input, unsigned char * output, unsigned char size)
{
   unsigned int i = 0, j = 0;
   unsigned int window = 0;
   unsigned char temp = 0;

   for (i = 0; i < size; i++)
   {
      temp = input[i];

      for (j = window + 8; j > window; j--)
      {
    	  output[j - 1] = (temp % 2) + 0x30;
    	  temp = temp >> 1;
      }

      window += 8;
   }
}

void Convert_To_ASCII_Hex (unsigned char * input, unsigned char * output, unsigned char size)
{
	unsigned int index = 0;

	for (index = 0; index < size; index++)
	{
		if ((input[index] & 0x0F) > 9)
		   output[index << 1] = (input[index] & 0x0F) + 0x37;
		else
		   output[index << 1] = (input[index] & 0x0F) + 0x30;

		if (((input[index] >> 4) & 0x0F) > 9)
           output[(index << 1) + 1] = ((input[index] >> 4) & 0x0F) + 0x37;
		else
		   output[(index << 1) + 1] = ((input[index] >> 4) & 0x0F) + 0x30;
	}
}

void Byte_Reverse (unsigned char * data, unsigned char size)
{
   unsigned int index = 0;

   unsigned char * temp = (unsigned char *)malloc(size);

   for (index = 0; index < size; index += 2)
   {
      temp[index] = data[size - index - 2];
      temp[index + 1] = data[size - index - 1];
   }

   for (index = 0; index < size; index++)
   {
	   data[index] = temp[index];
   }

   free(temp);
}
