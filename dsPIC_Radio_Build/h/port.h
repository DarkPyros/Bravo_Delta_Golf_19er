#ifndef	port_H
#define	port_H

/* I/O pins for reading mode signal */
#define PLAYBACK_TRIS	_TRISB6
#define RECORD_TRIS		_TRISB7

#define PLAYBACK_FLAG	_RB6
#define RECORD_FLAG		_RB7

#define START_FLAG_TRIS	_TRISB2
#define	START_FLAG_PIN	_RB2

/* Active mode for mode signal pins is LOW logic*/
#define ACTIVE_MODE		1


#if defined USE_SFM_CHIP
	#define REGULATOR_CONTROL_TRIS		TRISBbits.TRISB5
	#define REGULATOR_CONTROL_ANPCFG	AD1PCFGLbits.PCFG5
	#define REGULATOR_CONTROL_LAT		LATBbits.LATB5
	#define REGULATOR_WAKE_UP_DELAY	0x800
#else
	/* Output pin for synchronization timer */
	#define SYNC_CLK_PULSE_TRIS		_TRISB5
	#define SYNC_CLK_PULSE_PIN		_LATB5
#endif

#if defined TIMING_TEST
	#define 	TIMING_PULSE_TRIS	_TRISC15
	#define 	TIMING_PULSE_PIN 	_LATC15
#endif

#endif
