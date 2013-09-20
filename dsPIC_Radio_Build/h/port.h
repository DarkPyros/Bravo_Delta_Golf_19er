#ifndef	port_H
#define	port_H

/* I/O pins for reading mode signal */
#define PLAYBACK_TRIS	_TRISB6
#define RECORD_TRIS		_TRISB7

#define PLAYBACK_FLAG	_LATB6
#define RECORD_FLAG		_LATB7

/* Active mode for mode signal pins is LOW logic*/
#define ACTIVE_MODE		0

/* Output pin for synchronization timer */
#define SYNC_CLK_PULSE_TRIS		_TRISB5
#define SYNC_CLK_PULSE_PIN		_LATB5

#endif
