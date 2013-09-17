/*------------------------------------------------------------------*-
  Port.H (v1.00)
  ------------------------------------------------------------------

  Port Header for the project cc430_scheduler

-*------------------------------------------------------------------*/

#ifndef PORT_H_
#define PORT_H_

#include "../Main.h"

// ------ hSCH.C ----------------------------------------

// Comment this line out if error reporting is NOT required
//#define SCH_REPORT_ERRORS

#ifdef SCH_REPORT_ERRORS
// The port on which error codes will be displayed
// ONLY USED IF ERRORS ARE REPORTED
#define Error_port P1

#endif

// ------ Port.C ----------------------------------------

// Comment this line out if timer tick from Timer_A0 is NOT required
// Sourcing the timer tick from Timer_A0 should only be required during
// development.
//#define TIMER_TICK_TA0

#ifdef TIMER_TICK_TA0
	void Timer_A0_Init (void);
#endif

// Comment this line out if outputting the MCLK, SMCLK, and ACLK signals
// is NOT required.
// Outputting the clock signals should only be required during
// development.
//#define DEBUG_CLOCK_OUTPUT

void Clock_Init(void);

#endif /* PORT_H_ */

/*------------------------------------------------------------------*-
---- END OF FILE ------------------------------------------------
-*------------------------------------------------------------------*/
