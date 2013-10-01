#include "..\h\includes.h"

/* TIMER1 is operating at 25 MHz for some reason. 
 * I've gone ahead and programmed all the time 
 * delays and time-outs based on this. It will 
 * be updated when I determine the cause.
 */
void TIMER_timer1Start(int timeOut) {
	T1CONbits.TON = 0; /* Disable Timer */
	T1CONbits.TCS = 0; /* Select internal instruction cycle clock */
	T1CONbits.TGATE = 0; /* Disable Gated Timer mode */
	T1CONbits.TCKPS = 0b00; /* Select Prescaler 
							 * 00 == 1:1	01 == 1:8 
							 * 10 == 1:64 	11 == 1:256 */
	TMR1 = 0x0000; /* Clear timer register */
	PR1 = timeOut; /* Load the period value */
	IFS0bits.T1IF = 0; /* Clear Timer1 Interrupt Flag */
	T1CONbits.TON = 1; /* Start Timer */
} /* End of TIMER_timer1Start() */

void TIMER_timer1Stop() {
	_TON = 0;	/* Disable Timer */
	_T1IF = 0;	/* Clear Timer1 Interrupt Flag */
}

/******* End of File *******/
