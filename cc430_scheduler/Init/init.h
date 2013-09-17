/*------------------------------------------------------------------*-
  Init.H (v1.00)
  ------------------------------------------------------------------

  Board initialization header for the project cc430_scheduler

-*------------------------------------------------------------------*/

#ifndef INIT_H_
#define INIT_H_

#include "../Main.h"
#include "../RF1A/RF1A.h"

// ------ hSCH.C ----------------------------------------

// Comment this line out if error reporting is NOT required
//#define SCH_REPORT_ERRORS

#ifdef SCH_REPORT_ERRORS
// The port on which error codes will be displayed
// ONLY USED IF ERRORS ARE REPORTED
#define Error_port P1OUT

#endif

// ------ Port.C ----------------------------------------

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
