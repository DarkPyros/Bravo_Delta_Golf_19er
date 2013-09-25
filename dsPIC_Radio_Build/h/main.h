#ifndef main_H
#define main_H

#define TRUE 1
#define FALSE 0

/* Uncomment to user SFM chip */
//#define 	USE_SFM_CHIP
/* Uncomment to remove SPI buffer while loop */
//#define 	NO_WAIT_FOR_SPI

#define 	TIMING_TEST
//#define 	TIMING_SCH_UPDATE
//#define 	TIMING_MODE_SELECT
//#define 	TIMING_ENCODE_DATA
//#define 	TIMING_DECODE_DATA
//#define	TIMING_WRITE_SPI
#define 	TIMING_READ_SPI
//#define 	TIMING_WM_READ
//#define 	TIMING_WM_WRITE

/* Define operating modes of the system */
enum modes {
	PLAYBACK, 
	RECORD, 
	IDLE
};

extern enum modes 	modeFlag;
extern enum modes	currentMode;

extern int transceiverReady;

/* Task delay times*/
#define	DELAY_PLAYBACK_SAMPLING		0
#define	DELAY_READ_SPI				70
#define DELAY_DECODE_DATA			72
#define	DELAY_WM8510WRITE			15

#define	DELAY_RECORD_SAMPLING		0
#define	DELAY_WM8510READ			83
#define	DELAY_ENCODE_DATA			84
#define DELAY_WRITE_SPI				161

#define	DELAY_MODE_SELECT	0
#define DELAY_CHANGE_CODEC_SAMPLING		20

/* The number of timer ticks in each period.
 * This value is equal to the number of audio samples.
 */
#define FRAME_PERIOD	79

#endif
