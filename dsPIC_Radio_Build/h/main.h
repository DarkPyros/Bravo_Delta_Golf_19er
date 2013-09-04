#ifndef main_H
#define main_H

#define TRUE 1
#define FALSE 0

#define PLAYBACK_MODE 0
#define RECORD_MODE 1
#define IDLE 2

/* I/O pins for reading mode signal */
#define PLAYBACK_TRIS	_TRISB6
#define RECORD_TRIS		_TRISB7

#define PLAYBACK_FLAG	_RB6
#define RECORD_FLAG		_RB7

/* Flags
 * modeSelect - indicates if radio is in transmit mode
 * */
extern int modeSelect;

#endif
