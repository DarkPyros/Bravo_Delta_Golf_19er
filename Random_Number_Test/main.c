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

//#define TEXT

#define HWREG(x)     	(*((volatile unsigned int *)(x)))

#define STATUS_SUCCESS  0x01
#define STATUS_FAIL     0x00

#define OFS_AESACTL0           (0x0000)       /* AES accelerator control register 0 */
#define OFS_AESACTL0_L         OFS_AESACTL0
#define OFS_AESACTL0_H         OFS_AESACTL0+1
#define OFS_AESASTAT           (0x0004)       /* AES accelerator status register */
#define OFS_AESASTAT_L         OFS_AESASTAT
#define OFS_AESASTAT_H         OFS_AESASTAT+1
#define OFS_AESAKEY            (0x0006)       /* AES accelerator key register */
#define OFS_AESAKEY_L          OFS_AESAKEY
#define OFS_AESAKEY_H          OFS_AESAKEY+1
#define OFS_AESADIN            (0x0008)       /* AES accelerator data in register */
#define OFS_AESADIN_L          OFS_AESADIN
#define OFS_AESADIN_H          OFS_AESADIN+1
#define OFS_AESADOUT           (0x000A)       /* AES accelerator data out register  */
#define OFS_AESADOUT_L         OFS_AESADOUT
#define OFS_AESADOUT_H         OFS_AESADOUT+1

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

void Board_Init(void);
void Clock_Init(void);
void RTC_Init(void);
void UART_Init(void);
void UART_TX(unsigned char *, unsigned char);
void Convert_To_ASCII_Binary(unsigned char *, unsigned char *);
unsigned char AES_setCipherKey (unsigned int,const unsigned char *);
unsigned char AES_encryptData (unsigned int,const unsigned char *,unsigned char *);
unsigned char AES_decryptDataUsingEncryptionKey (unsigned int,const unsigned char *, unsigned char *);



int main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  // Init board and clock
  Board_Init();
  Clock_Init();
  RTC_Init();
  UART_Init();

  TA1CTL = TASSEL_1 + MC_2 + TACLR;	//Reset and activate Counter



  	unsigned char CipherKey[16] = 	{	0x2b, 0x7e, 0x15, 0x16,
  										0x28, 0xae, 0xd2, 0xa6,
  										0xab, 0xf7, 0x15, 0x88,
  										0x09, 0xcf, 0x4f, 0x3c	};

/*
  	unsigned char Data[16] = 		{	0x6b, 0xc1, 0xbe, 0xe2,
  										0x2e, 0x40, 0x9f, 0x96,
  										0xe9, 0x3d, 0x7e, 0x11,
  										0x73, 0x93, 0x17, 0x2a	};
*/


  struct nonce RandomData;

  unsigned char Button_Press = 0;

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
		__delay_cycles(6000000); // Switch debounce = 300ms @ 20 MHz

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
    	RandomData.somebits += (unsigned long long)TA1R;
    	TA1CTL = TASSEL_1 + MC_2 + TACLR;		//Reset and activate Counter
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

void Clock_Init(void)
{
  // Setup Clock
  P5SEL |= BIT0 + BIT1;                     // Port select XT1
  UCSCTL6 |= XCAP_3 + XT1OFF + XT2OFF;      // Internal load cap
                                            // XT1 and XT2 off
  // Loop until XT1 & DCO stabilizes - In this case loop until XT1 and DCO settle
  do
  {
	UCSCTL7 &= ~(XT1LFOFFG + DCOFFG);
                                            // Clear XT1,DCO fault flags
	SFRIFG1 &= ~OFIFG;                  	// Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive
                                            // strength

  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P2MAP0 = PM_ACLK;                         // Map ACLK output to P2.0
  P2MAP2 = PM_MCLK;                         // Map MCLK output to P2.2
  P2MAP4 = PM_SMCLK;                        // Map SMCLK output to P2.4
  PMAPPWD = 0;                              // Lock port mapping registers

  P2DIR |= BIT0 + BIT2 + BIT4;              // ACLK, MCLK, SMCLK set out to pins
  P2SEL |= BIT0 + BIT2 + BIT4;              // P2.0,2,4 for debugging purposes.

  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_7;                      // Select DCO range 20MHz operation
  UCSCTL2 = FLLD_1 + 614;                   // Set DCO Multiplier for 12MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (614 + 1) * 32768 = 20MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 20 MHz / 20,000 Hz = 625000 = MCLK cycles for DCO to settle
  __delay_cycles(625000);

  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

}

void Board_Init(void)
{
  PMMCTL0_H = PMMPW_H;                      // open PMM
  PM5CTL0 &= ~LOCKIO;                       // Clear LOCKBAK and enable ports
  PMMCTL0_H = 0x00;                         // close PMM

  // Port Configuration
  P1OUT = 0x80;P2OUT = 0x00;P3OUT = 0x00;P4OUT = 0x00;P5OUT = 0x00;PJOUT = 0x00;
  P1DIR = 0x6F;P2DIR = 0xFF;P3DIR = 0xFF;P4DIR = 0xFF;P5DIR = 0xFF;PJDIR = 0xFF;
  P1REN = 0x90;
}

void RTC_Init(void)
{
  // Configure RTC_D
  RTCCTL01 &= ~(RTCTEVIFG);                //Clear event interrupt
  RTCCTL01 |= RTCTEVIE + RTCHOLD;         //RTC hold, enable RTC
                                          // event interrupt
  RTCYEAR = 2013;                         // Year = 2011
  RTCMON = 07;                            // Month = 10 = October
  RTCDAY = 21;                            // Day = 7 = 7th
  RTCDOW = 05;                            // Day of week = 5 = Friday
  RTCHOUR = 22;                           // Hour = 11
  RTCMIN = 59;                            // Minute = 59
  RTCSEC = 55;                            // Seconds = 55
  //RTCCTL01|= RTCTEV_0;                      // Set RTCTEV for 1 minute alarm
  RTCCTL01 &= ~(RTCHOLD);                   // Start RTC calendar mode
}

void UART_Init (void)
{
   P1SEL |= BIT5 + BIT6;

   UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
   UCA0CTL0 = 0x00;                          // UART - 8/N/1 with LSB first
   UCA0CTL1 |= UCSSEL_2;                     // SMCLK
   UCA0BR0 = 0x5B;                           // 20MHz 57600 (see User's Guide)
   UCA0BR1 = 1;                              // 20MHz 57600
   UCA0MCTL |= UCBRS_2 + UCBRF_0;            // Modulation UCBRSx=5, UCBRFx=0
   UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void UART_TX(unsigned char * randData, unsigned char size)
{
   unsigned int index = 0;

   for (index = 0; index < size; index++)
   {
	   while (UCA0STAT & UCBUSY);   // wait for UART to be free

	   UCA0TXBUF = randData[index];	// transmit random byte data
   }

   while (UCA0STAT & UCBUSY);   // wait for UART to be free

   UCA0TXBUF = 0x0D;	// transmit CR

   while (UCA0STAT & UCBUSY);   // wait for UART to be free

   UCA0TXBUF = 0x0A;	// transmit LF
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

unsigned char AES_setCipherKey (unsigned int baseAddress,
	 const unsigned char * CipherKey
	 )
{
	unsigned char i = 0;
	unsigned int tempVariable = 0;

	// Wait until AES accelerator is busy
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	for (i = 0; i < 16; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESAKEY) = ( unsigned int)(( unsigned int)CipherKey[i] | ( unsigned int) (CipherKey[i + 1] << 8));
		tempVariable = (unsigned int)(CipherKey[i]);
		tempVariable = tempVariable | ((unsigned int)(CipherKey[i + 1]) << 8);
		HWREG(baseAddress + OFS_AESAKEY) = tempVariable;
	}

    // Wait until key is written
	while(0x00 == (HWREG(baseAddress + OFS_AESASTAT) & AESKEYWR ));

    return STATUS_SUCCESS;
}

unsigned char AES_encryptData (unsigned int baseAddress,
	const unsigned char * Data,
	unsigned char * encryptedData)
{
	unsigned char i;
	unsigned int tempData = 0;
	unsigned int tempVariable = 0;

	// Set module to encrypt mode
	HWREG(baseAddress + OFS_AESACTL0) &= ~AESOP_3;


	// Write data to encrypt to module
	for (i = 0; i < 16; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESADIN) = ( unsigned int)(( unsigned int)Data[i] | ( unsigned int) (Data[i + 1] << 8));
		tempVariable = (unsigned int)(Data[i]);
		tempVariable = tempVariable | ((unsigned int)(Data[i+1]) << 8);
		HWREG(baseAddress + OFS_AESADIN) = tempVariable;
	}

	// Key that is already written shall be used
	// Encryption is initialized by setting AESKEYWR to 1
	HWREG(baseAddress + OFS_AESASTAT) |= AESKEYWR;

	// Wait unit finished ~167 MCLK
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	// Write encrypted data back to variable
	for (i = 0; i < 16; i = i + 2)
	{
		tempData = HWREG(baseAddress + OFS_AESADOUT);
		*(encryptedData + i) = (unsigned char)tempData;
		*(encryptedData +i + 1) = (unsigned char)(tempData >> 8);


	}

    return STATUS_SUCCESS;
}

unsigned char AES_decryptDataUsingEncryptionKey (unsigned int baseAddress,
	const unsigned char * Data,
	unsigned char * decryptedData)
{
	unsigned char i;
	unsigned int tempData = 0;
	unsigned int tempVariable = 0;

	// Set module to decrypt mode
	HWREG(baseAddress + OFS_AESACTL0) &= ~(AESOP1);
	HWREG(baseAddress + OFS_AESACTL0) |= AESOP0;

	// Write data to decrypt to module
	for (i = 0; i < 16; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESADIN) = ( unsigned int)(( unsigned int)Data[i] | ( unsigned int) (Data[i + 1] << 8));
		tempVariable = (unsigned int)(Data[i+1]  << 8);
		tempVariable = tempVariable | ((unsigned int)(Data[i]));
		HWREG(baseAddress + OFS_AESADIN) = tempVariable;
	}

	// Key that is already written shall be used
	// Now decryption starts
	HWREG(baseAddress + OFS_AESASTAT) |= AESKEYWR;

	// Wait unit finished ~214 MCLK
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	// Write encrypted data back to variable
	for (i = 0; i < 16; i = i + 2)
	{
		tempData = HWREG(baseAddress + OFS_AESADOUT);
		*(decryptedData + i) = (unsigned char)tempData;
		*(decryptedData +i+1) = (unsigned char)(tempData >> 8);
	}

	return STATUS_SUCCESS;
}
