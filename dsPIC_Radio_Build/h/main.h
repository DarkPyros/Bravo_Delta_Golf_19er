#ifndef main_H
#define main_H

#define TRUE 1
#define FALSE 0

/* Define operating modes of the system */
enum modes {
	PLAYBACK, 
	RECORD, 
	IDLE
};

/* Task delay times*/
#define	DELAY_PLAYBACK_SAMPLING		0
#define	DELAY_READ_SPI				4
#define DELAY_DECODE_DATA			9
#define	DELAY_WM8510WRITE			15

#define	DELAY_RECORD_SAMPLING		0
#define	DELAY_WM8510READ			83
#define	DELAY_ENCODE_DATA			84
#define DELAY_WRITE_SPI				161

#define DELAY_CHANGE_CODEC_SAMPLING		20
#define	DELAY_MODE_SELECT	0

/* The number of timer ticks in each period.
 * This value is equal to the number of audio samples.
 */
#define FRAME_PERIOD	80

#endif
