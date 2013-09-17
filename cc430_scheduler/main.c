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
#include "Main.h"
#include "Init.h"
#include "hSCH.h"
#include "hSCH430.h"

/* ............................................................... */
/* ............................................................... */

void main(void) {

    // Setup the clock and ports
	Init();

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
