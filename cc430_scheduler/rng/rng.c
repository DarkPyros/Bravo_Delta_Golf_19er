/*------------------------------------------------------------------*-
  Rng.C (v1.00)
  ------------------------------------------------------------------

  The random number generator uses the AES module of the CC430 and
  a nonce (number used once) to generate randomo numbers which are
  used to choose the next channel.

-*------------------------------------------------------------------*/

#include "rng.h"

tNONCE Nonce;

static tByte Random_Numbers[AES_SIZE] = { 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00 };

static tByte Pearson_Hash[256] = { 1, 87, 49, 12, 176, 178, 102, 166, 121, 193, 6, 84, 249, 230, 44, 163,
								  14, 197, 213, 181, 161, 85, 218, 80, 64, 239, 24, 226, 236, 142, 38, 200,
								  110, 177, 104, 103, 141, 253, 255, 50, 77, 101, 81, 18, 45, 96, 31, 222,
								  25, 107, 190, 70, 86, 237, 240, 34, 72, 242, 20, 214, 244, 227, 149, 235,
								  97, 234, 57, 22, 60, 250, 82, 175, 208, 5, 127, 199, 111, 62, 135, 248,
								  174, 169, 211, 58, 66, 154, 106, 195, 245, 171, 17, 187, 182, 179, 0, 243,
								  132, 56, 148, 75, 128, 133, 158, 100, 130, 126, 91, 13, 153, 246, 216, 219,
								  119, 68, 223, 78, 83, 88, 201, 99, 122, 11, 92, 32, 136, 114, 52, 10,
								  138, 30, 48, 183, 156, 35, 61, 26, 143, 74, 251, 94, 129, 162, 63, 152,
								  170, 7, 115, 167, 241, 206, 3, 150, 55, 59, 151, 220, 90, 53, 23, 131,
								  125, 173, 15, 238, 79, 95, 89, 16, 105, 137, 225, 224, 217, 160, 37, 123,
								  118, 73, 2, 157, 46, 116, 9, 145, 134, 228, 207, 212, 202, 215, 69, 229,
								  27, 188, 67, 124, 168, 252, 42, 4, 29, 108, 21, 247, 19, 205, 39, 203,
								  233, 40, 186, 147, 198, 192, 155, 33, 164, 191, 98, 204, 165, 180, 117, 76,
								  140, 36, 210, 172, 41, 54, 159, 8, 185, 232, 113, 196, 231, 47, 146, 120,
								  51, 65, 28, 144, 254, 221, 93, 189, 194, 139, 112, 43, 71, 109, 184, 209 };

void RNG_Init (void)
{
	// Setup the nonce timer
	Nonce_Timer_Init();

	Nonce_Init();
}

tByte RNG_Get_Rand (void)
{
	//LED_ON;
	// 5 us
	Update_Nonce();
	//LED_OFF;

	//LED_ON;
	// 12.8 us
	AES_Encrypt_Data(AES_ADDR, (tByte *) &Nonce, Random_Numbers);
	//LED_OFF;

	tByte i;
	tByte h = 0;
	tByte index = 0;

	for (i = 0; i < sizeof(Random_Numbers); i++)
	{
		index = h ^ Random_Numbers[i];
		h = Pearson_Hash[index];
	}

	return (h >> 1);
}

tByte * RNG_Get_Nonce (void)
{
	return (tByte *)&Nonce;
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

void Overwrite_Nonce (tByte const * const New_Nonce)
{
	tByte i;
	tByte * nonce = RNG_Get_Nonce();

	for (i = 0; i < AES_SIZE; i++)
	{
		nonce[i] = New_Nonce[i];
	}
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
