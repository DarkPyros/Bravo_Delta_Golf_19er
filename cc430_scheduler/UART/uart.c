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

static tByte UART_TX_Buffer[UART_BUFFER_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												  0x00, 0x00, 0x00, 0x00, 0x00 };
static tByte UART_TX_Flag = UART_TX_FLAG_DOWN;
static tByte UART_TX_Length;

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

/*------------------------------------------------------------------*-

  UART_TX()

  UART transmit function. Transmits the current character in the
  UART_TX_Buffer pointed to by the index variable. This is done so
  that the UART_TX_Buffer can be transmitted one character at a time
  without having the CPU wait while the each character is
  transmitted.

-*------------------------------------------------------------------*/
void UART_TX (void)
{
	static tByte index = 0;

	// if the UART is free
	if (!(UCA0STAT & UCBUSY) && (UART_TX_Flag == UART_TX_FLAG_DOWN))
	{
	 	// transmit a byte of data
		UCA0TXBUF = UART_TX_Buffer[UART_TX_Length - index - 1];
	    index++;

	    if (index >= UART_TX_Length)
	    {
	        index = 0;
	        UART_TX_Length = 0;
	        UART_TX_Flag = UART_TX_FLAG_UP;
	    }
	}
}

/*------------------------------------------------------------------*-

  UART_TX_Buffer_Write()

  UART transmit buffer write function. Writes a string to the
  UART_TX_Buffer. This is done so that the program can 'send'
  information to the UART without having to wait for the entire
  string to be transmitted over the UART connection.

-*------------------------------------------------------------------*/
void UART_TX_Buffer_Write (tByte const * pStr, tByte length)
{
    tByte i = 0;

    if (UART_TX_Flag == UART_TX_FLAG_UP)
    {
        if (length > (UART_BUFFER_SIZE - 2))
        	length = (UART_BUFFER_SIZE - 2);

        UART_TX_Buffer[0] = 0x0D;
        UART_TX_Buffer[1] = 0x0A;

        for (i = 2; i < length + 2; i++)
        {
            UART_TX_Buffer[i] = pStr[i - 2];
        }

        UART_TX_Length = length + 2;

        UART_TX_Flag = UART_TX_FLAG_DOWN;
    }
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
			output[(index << 1) + 1]= ((input[index] >> 4) & 0x0F) + 0x37;
		else
			output[(index << 1) + 1] = ((input[index] >> 4) & 0x0F) + 0x30;
	}
}

void Byte_Reverse (tByte * const data, tByte const size)
{
   tWord index = 0;

   tByte * temp = (tByte *)malloc(size);

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
