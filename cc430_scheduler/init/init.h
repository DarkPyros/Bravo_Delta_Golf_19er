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
// is NOT required.
// Outputting the clock signals should only be required during
// development.
//#define DEBUG_CLOCK_OUTPUT

void Init (void);
void Clock_Init (void);
void RTC_Init (void);
void Timer_Init (void);
void Timer_Reset (void);

#endif /* INIT_H_ */

/*------------------------------------------------------------------*-
---- END OF FILE ------------------------------------------------
-*------------------------------------------------------------------*/
