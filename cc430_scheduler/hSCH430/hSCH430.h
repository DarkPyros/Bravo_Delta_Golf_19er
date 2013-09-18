/*------------------------------------------------------------------*-
  hSCH430.h (v1.00)
  ------------------------------------------------------------------

- see hSCH430.C for details

-*------------------------------------------------------------------*/

#ifndef HSCH430_H_
#define HSCH430_H_

#include "../main.h"
#include "../hSCH/hSCH.H"

// ------ Public function prototypes -------------------------------

void hSCH_Init_P1_1(void);
void hSCH_Start(void);

// hSCH430.C also includes the Port 1 ISR setup as
// the timer tick source.
//
// Port 1 interrupt service routine
// #pragma vector=PORT1_VECTOR
// __interrupt void hSCH_Update(void)

#endif /* HSCH430_H_ */

/*------------------------------------------------------------------*-
---- END OF FILE ------------------------------------------------
-*------------------------------------------------------------------*/
