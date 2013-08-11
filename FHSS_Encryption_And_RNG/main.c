#include <msp430.h>
#include "Includes/AES.h"
#include "Includes/Init.h"

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

  	unsigned char Initial[16] = 	{	0x00, 0x01, 0x02, 0x03,
  										0x04, 0x05, 0x06, 0x07,
  										0x08, 0x09, 0x0A, 0x0B,
  										0x0C, 0x0D, 0x0E, 0x0F	};

  	unsigned char InitialDe[16] = 	{	0x00, 0x01, 0x02, 0x03,
  										0x04, 0x05, 0x06, 0x07,
  										0x08, 0x09, 0x0A, 0x0B,
  										0x0C, 0x0D, 0x0E, 0x0F	};

  	unsigned char Data[16] =		{	0x6b, 0xc1, 0xbe, 0xe2,
  										0x2e, 0x40, 0x9f, 0x96,
  										0xe9, 0x3d, 0x7e, 0x11,
  										0x73, 0x93, 0x17, 0x2a	};


  struct nonce RandomData;

  unsigned char Button_Press = 0;
  unsigned int Timer = 0;

  AES_setCipherKey(__MSP430_BASEADDRESS_AES__, CipherKey);
  AES_encryptDataBlock(__MSP430_BASEADDRESS_AES__, Data, DataAESencrypted, Initial, 16);
  AES_generateFirstRoundKey(__MSP430_BASEADDRESS_AES__, CipherKey);
  AES_decryptDataBlock(__MSP430_BASEADDRESS_AES__, DataAESencrypted, DataAESdecrypted, InitialDe, 16);

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

	  /*
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
*/


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


