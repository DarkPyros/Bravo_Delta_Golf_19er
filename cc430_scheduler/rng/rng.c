/*------------------------------------------------------------------*-
  Rng.C (v1.00)
  ------------------------------------------------------------------

  The random number generator uses the AES module of the CC430 and
  a nonce (number used once) to generate randomo numbers which are
  used to choose the next channel.

-*------------------------------------------------------------------*/

#include "rng.h"

static tNONCE Nonce;

static tByte Random_Numbers[AES_SIZE] = { 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00 };

void RNG_Init (void)
{
	// Setup the nonce timer
	Nonce_Timer_Init();

	Nonce_Init();
}

void RNG_Generate (void)
{
	//LED_ON;
	// 5 us
	Update_Nonce();
	//LED_OFF;

	//LED_ON;
	// 12.8 us
	AES_Start_Encryption(AES_ADDR, (tByte *) &Nonce);
	//LED_OFF;
}

void RNG_Harvest (void)
{
	tByte i;

	//LED_ON;
	AES_Get_Encrypted_Data(AES_ADDR, Random_Numbers);
	//LED_OFF;

	//LED_ON;
	for (i = 0; i < AES_SIZE; i++)
	{
		Random_Numbers[i] = Random_Numbers[i] >> 1;
	}
	//LED_OFF;
}

tByte RNG_Get_Rand (void)
{
	tByte rand;
	static tByte RNG_Index = 0;

	rand = Random_Numbers[RNG_Index];

	if (++RNG_Index == AES_SIZE)
		RNG_Index = 0;

	return rand;
}

void Nonce_Init (void)
{
	Nonce.year = (tWord)RTCYEAR;
	Nonce.month = (tByte)RTCMON;
	Nonce.day = (tByte)RTCDAY;
	Nonce.hour = (tByte)RTCHOUR;
	Nonce.minute = (tByte)RTCMIN;
	Nonce.second = (tByte)RTCSEC;
	Nonce.seed_channel = 0xAA;
	Nonce.counter = 0xAAAAAAAAAAAAAAAALL;
}

void Update_Nonce (void)
{
	tWord timer;

	Nonce.year = (tWord)RTCYEAR;
	Nonce.month = (tByte)RTCMON;
	Nonce.day = (tByte)RTCDAY;
	Nonce.hour = (tByte)RTCHOUR;
	Nonce.minute = (tByte)RTCMIN;
	Nonce.second = (tByte)RTCSEC;

	do
	{
		timer = TA1R;
	} while (timer != TA1R);

	Nonce.counter += timer;

	Nonce_Timer_Reset();
}

/*------------------------------------------------------------------*-

  Nonce_Timer_Init()

  Initialize Timer_TA1 for the microsecond nonce counter

-*------------------------------------------------------------------*/
void Nonce_Timer_Init (void)
{
	// Reset and activate timer TA1
	Nonce_Timer_Reset();
}

/*------------------------------------------------------------------*-

  Nonce_Timer_Reset()

  Reset Timer_TA1 for the microsecond nonce counter

-*------------------------------------------------------------------*/
void Nonce_Timer_Reset (void)
{
	// Set TAIDEX to 2 for a clock divider of /3
	TA1EX0 = TAIDEX_2;

	// TASSEL_2 = set timer clock source to SMCLK
	// ID_2 = set clock divider to /4, along with TAIDEX for
	//        a total clock divider of /12
	// MC_2 = continuous up counting
	// TACLR = resets and activates the timer counter
	TA1CTL = TASSEL_2 + ID_2 + MC_2 + TACLR;	//Reset and activate Counter
}
