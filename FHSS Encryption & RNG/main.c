/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//   CC430x614x Demo - RTC_D, LPM3.5, & alarm
//
//   Description: The RTC_D module is used to set the time, start RTC operation,
//   and read the time from the respective RTC registers. Software will set the
//   original time to 11:59:55 am on Friday October 7, 2011. Then the RTC will
//   be activated through software, and an alarm will be created for the next
//   minute (12:00:00 pm). The device will then enter LPM3.5 awaiting
//   the event interrupt. The device will flash the LED and then reset the RTC
//   to the time above upon being woken up by the event.
//
//   NOTE: This code example was created and tested using the EM430F6147RF900
//   Rev3.4 board. To ensure that LPM3.5 is entered and exited properly:
//
//   1. In debug mode:  enable  LPM3.5 debug option for the project and do not
//      select "Release JTAG on Go” option in IAR and “Free Run” option in CCS.
//
//   2. In stand-alone mode (JTAG disconnected, for accurate current measurements):
//      remove JTAG after programming device and externally power board. A reset
//      may be required after removing the JTAG. When device is in LPM3.5 ~1uA
//      should be measured on JP2.
//
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                CC430x614x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|--> LED
//
//   G. Larmore
//   Texas Instruments Inc.
//   June 2012
//   Built with IAR Embedded Workbench V5.40.1 & Code Composer Studio V5.2
//******************************************************************************

#include <msp430.h>
#include "AES.h"
#include "Init.h"

#define TEXT
//#define DEBUG



struct nonce{
	int year;
	char month;
	char day;
	char hour;
	char minute;
	char second;
	char seed_channel;
	unsigned long long somebits;
};

unsigned char RandomNumbers[16] = {	0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00	};

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

unsigned char DataAESdecrypted[16] = {	0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00	};

unsigned char DataAESencrypted[16] = {	0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00	};


void UART_TX(unsigned char *, unsigned char);
void Convert_To_ASCII_Binary(unsigned char *, unsigned char *);




int main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  // Init board and clock
  Board_Init();
  Clock_Init();
  RTC_Init();
  UART_Init();

  TA1EX0 = 0x0004;
  TA1CCTL0 = 0x0080;
  TA1CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;	//Reset and activate Counter



  	unsigned char CipherKey[16] = 	{	0x2b, 0x7e, 0x15, 0x16,
  										0x28, 0xae, 0xd2, 0xa6,
  										0xab, 0xf7, 0x15, 0x88,
  										0x09, 0xcf, 0x4f, 0x3c	};


  struct nonce RandomData;

  unsigned char Button_Press = 0;
  unsigned int Timer = 0;

  AES_setCipherKey(__MSP430_BASEADDRESS_AES__, CipherKey);

  RandomData.year = (int)RTCYEAR;
  RandomData.month = (char)RTCMON;
  RandomData.day = (char)RTCDAY;
  RandomData.hour = (char)RTCHOUR;
  RandomData.minute = (char)RTCMIN;
  RandomData.second = (char)RTCSEC;
  RandomData.seed_channel = 0xAA;
  RandomData.somebits = 0xAAAAAAAAAAAAAAAALL;

  while(1)
  {
	if ((P1IN & BIT7) == 0)
	{
        #ifndef DEBUG
		   __delay_cycles(6000000); // Switch debounce = 300ms @ 20 MHz
        #endif

		if (Button_Press == 0)
			Button_Press = 1;
		else
			Button_Press = 0;
	}


    if (Button_Press == 1)
    {
    	P1OUT ^= BIT0;
    	RandomData.year = (int)RTCYEAR;
    	RandomData.month = (char)RTCMON;
    	RandomData.day = (char)RTCDAY;
    	RandomData.hour = (char)RTCHOUR;
    	RandomData.minute = (char)RTCMIN;
    	RandomData.second = (char)RTCSEC;

    	do
    	{
    	   Timer = TA1R;
    	} while (Timer != TA1R);

    	RandomData.somebits += Timer;
    	TA1CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;	//Reset and activate Counter
    	AES_encryptData(__MSP430_BASEADDRESS_AES__, (unsigned char *) &RandomData, RandomNumbers);
    	//AES_decryptDataUsingEncryptionKey(__MSP430_BASEADDRESS_AES__, RandomNumbers, DataAESdecrypted);
    	P1OUT ^= BIT0;

        #ifdef TEXT
    	   Convert_To_ASCII_Binary(RandomNumbers, BinaryText);
    	   UART_TX(BinaryText, sizeof(BinaryText));
        #else
    	   UART_TX(RandomNumbers, sizeof(RandomNumbers));
        #endif
    }

	//for(i = 0; i < 1000; i++);

  }
}



void UART_TX(unsigned char * randData, unsigned char size)
{
   unsigned int index = 0;

   for (index = 0; index < size; index++)
   {
	   while (UCA0STAT & UCBUSY);   // wait for UART to be free

	   UCA0TXBUF = randData[index];	// transmit random byte data
   }

   #ifdef TEXT
      while (UCA0STAT & UCBUSY);   // wait for UART to be free

      UCA0TXBUF = 0x0D;	// transmit CR

      while (UCA0STAT & UCBUSY);   // wait for UART to be free

      UCA0TXBUF = 0x0A;	// transmit LF
   #endif
}

void Convert_To_ASCII_Binary(unsigned char * randData, unsigned char * BinaryData)
{
   unsigned int i = 0, j = 0;
   unsigned int window = 0;
   unsigned char temp = 0;

   for (i = 0; i < 16; i++)
   {
      temp = randData[i];

      for (j = window + 8; j > window; j--)
      {
    	  BinaryData[j - 1] = (temp % 2) + 0x30;
    	  temp = temp >> 1;
      }

      window += 8;
   }
}


