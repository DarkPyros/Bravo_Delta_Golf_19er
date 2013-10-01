/*
 * uart.c
 *
 *  Created on: Sep 30, 2013
 *      Author: Chris
 */

#include "uart.h"

tByte BinaryText[128] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
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
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

tByte HexText[64] = { 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00 };

tByte Timer_Hex[4] = { 0x00, 0x00, 0x00, 0x00 };

void UART_Init (void)
{
	PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
	PMAPCTL = PMAPRECFG;					  // Enable further port remaps
	P2MAP3 = PM_UCA0RXD;                      // Map UART RX input to P2.3
	P2MAP5 = PM_UCA0TXD;                      // Map UART TX output to P2.5
	PMAPPWD = 0;                              // Lock port mapping registers

	P2SEL |= BIT3 + BIT5;

	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL0 = 0x00;                          // UART - 8/N/1 with LSB first
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 112;                            // 13MHz 115200 (see User's Guide)
	UCA0BR1 = 0;                              // 13MHz 115200
	UCA0MCTL |= UCBRS_7;					  // UCBRS = 7
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void UART_TX (tByte const data)
{
	UCA0TXBUF = data;			// transmit a byte of data
}

tByte UART_TX_Busy (void)
{
	if (!(UCA0STAT & UCBUSY))	// if UART is free
		return FALSE;
	else
		return TRUE;
}

void Convert_To_ASCII_Binary (tByte const * const input, tByte * const output, tByte const size)
{
   tWord i = 0, j = 0;
   tWord window = 0;
   tByte temp = 0;

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

void Convert_To_ASCII_Hex (tByte const * const input, tByte * const output, tByte const size)
{
	tWord index = 0;

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

void Byte_Reverse (tByte * const data, tByte const size)
{
   tWord index = 0;

   tByte * const temp = (tByte *)malloc(size);

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
