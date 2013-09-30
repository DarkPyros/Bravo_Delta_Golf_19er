/*------------------------------------------------------------------*-
  Main.c (v1.00)
  ------------------------------------------------------------------

  /// HYBRID SCHEDULER ///

  Project to use a Texas Instruments CC430 as a frequency hopping
  radio that transmits and receives encrypted voice data. The project
  uses a hybrid scheduler with only one pre-emptive task allowed. The
  timer ticks are provided by a dsPIC chip at a rate of every 125 us.
  The dsPIC also handles voice sampling and playback, as well as voice
  data compression and decompression.

  *** All timing is in TICKS (not milliseconds) ***

-*------------------------------------------------------------------*/

#include "main.h"
#include "tasks.h"
#include "./init/init.h"
#include "./hSCH/hSCH.h"
#include "./hSCH430/hSCH430.h"

extern ROLE Role_Flag_G;

/* ............................................................... */
/* ............................................................... */

void main(void) {

    // Setup the clock and ports
	Init();

    // Setup the scheduler
    hSCH_Init_P1_1();

    // Add tasks here
    Schedule_Tasks();

    // Start the scheduler
    hSCH_Start();

    // Tell dsPIC we're ready for the timer tick
    Flags_Pulled_Low();

    while (1)
    {
    	hSCH_Dispatch_Tasks();
    }
}

/*------------------------------------------------------------------*-
---- END OF FILE ------------------------------------------------
-*------------------------------------------------------------------*/
