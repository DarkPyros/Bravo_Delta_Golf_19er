/*------------------------------------------------------------------*-
  Main.c (v1.00)
  ------------------------------------------------------------------

  /// HYBRID SCHEDULER ///

  Project to use a Texas Instruments CC430 as a frequency hopping
  radio that transmits and receives encrypted voice data. The project
  uses a hybrid scheduler with only one pre-emptive task allowed. The
  timer ticks are provided by a DsPIC chip at a rate of every 125 us.
  The DsPIC also handles voice sampling and playback, as well as voice
  data compression and decompression.

  *** All timing is in TICKS (not milliseconds) ***

-*------------------------------------------------------------------*/

#include <msp430.h>
#include "main.h"
#include "hSCH.h"
#include "hSCH430.h"

/* ............................................................... */
/* ............................................................... */

void main(void) {

    WDTCTL = WDTPW + WDTHOLD;	    // Stop watchdog timer

    // Setup the clock
    Clock_Init();

	// Use Timer_A0 to provide the timer
    // tick for development purposes
    #ifdef TIMER_TICK_TA0
    	Timer_A0_Init();
	#endif

    // Setup the scheduler
    hSCH_Init_P1_1();

    // Add tasks here

    // Start the scheduler
    hSCH_Start();

    while (1)
    {
    	hSCH_Dispatch_Tasks();
    }
}

/*------------------------------------------------------------------*-
---- END OF FILE ------------------------------------------------
-*------------------------------------------------------------------*/
