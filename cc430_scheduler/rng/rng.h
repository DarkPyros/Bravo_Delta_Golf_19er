/*------------------------------------------------------------------*-
  Rng.H (v1.00)
  ------------------------------------------------------------------

  The random number generator uses the AES module of the CC430 and
  a nonce (number used once) to generate randomo numbers which are
  used to choose the next channel.

-*------------------------------------------------------------------*/

#ifndef RNG_H_
#define RNG_H_

#include "../main.h"
#include "../AES/AES.h"

typedef struct S_NONCE {
	tWord year;
	tByte month;
	tByte day;
	tByte hour;
	tByte minute;
	tByte second;
	tByte seed_channel;
	tLongLong counter;
} tNONCE;

void RNG_Init (void);
void RNG_Generate (void);
void RNG_Harvest (void);
tByte RNG_Get_Rand (void);
void Nonce_Init (void);
void Update_Nonce (void);
void Nonce_Timer_Init (void);
void Nonce_Timer_Reset (void);

#endif /* RNG_H_ */
