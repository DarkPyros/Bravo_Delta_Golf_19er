/*------------------------------------------------------------------*-
  Init.H (v1.00)
  ------------------------------------------------------------------

  Board initialization header for the project cc430_scheduler

-*------------------------------------------------------------------*/

#ifndef INIT_H_
#define INIT_H_

#include "../main.h"

// ------ Init.C ----------------------------------------

// Comment this line out if timer tick from Timer_A0 is NOT required
// Sourcing the timer tick from Timer_A0 should only be required during
// development.
#define TIMER_TICK_TA0

#ifdef TIMER_TICK_TA0
	void Timer_A0_Init (void);
#endif

// Comment this line out if outputting the MCLK, SMCLK, and ACLK signals
// is NOT required. Outputting the clock signals should only be required
// during development.
//#define DEBUG_CLOCK_OUTPUT

void Init (void);
void Clock_Init (void);
void RTC_Init (void);
void Sandwich_Timer_Init (void);
void LED_Init (void);
void Flags_Init (void);
void Flags_Pulled_Low (void);

#define Sandwich_Timer_Overflow() (TA1CCTL0 & CCIFG)

#endif /* INIT_H_ */

/*------------------------------------------------------------------*-
---- END OF FILE ------------------------------------------------
-*------------------------------------------------------------------*/
