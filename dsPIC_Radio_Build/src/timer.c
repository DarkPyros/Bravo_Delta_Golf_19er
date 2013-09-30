#include "..\h\includes.h"

void TIMER_timer1Start(int timeOut) {
	T1CONbits.TON = 0; /* Disable Timer */
	T1CONbits.TCS = 0; /* Select internal instruction cycle clock */
	T1CONbits.TGATE = 0; /* Disable Gated Timer mode */
	T1CONbits.TCKPS = 0b00; /* Select 1:1 Prescaler */
	TMR1 = 0x0000; /* Clear timer register */
	PR1 = timeOut; /* Load the period value */
	IFS0bits.T1IF = 0; /* Clear Timer1 Interrupt Flag */
	T1CONbits.TON = 1; /* Start Timer */
}

void TIMER_timer1Stop() {
	_TON = 0;	/* Disable Timer */
	_T1IF = 0;	/* Clear Timer1 Interrupt Flag */
}

/******* End of File *******/
