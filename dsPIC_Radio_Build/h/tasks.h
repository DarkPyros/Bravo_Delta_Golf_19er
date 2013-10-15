#ifndef TASKS_H
#define TASKS_H

/* Define operating modes of the system */
enum modes {
	PLAYBACK, 
	RECORD, 
	IDLE
};

extern enum modes 	modeFlag;
extern enum modes	currentMode;

extern int transceiverReady;

/******* Public Constants *******/
/*   Task delay times
 * If tasks are added after scheduler begins, the
 * task will run (delay value + 1) ticks after
 * being added */
#define DELAY_IDLE_SAMPLING			0

#define	DELAY_PLAYBACK_SAMPLING		0
#define	DELAY_READ_SPI				2
#define DELAY_DECODE_DATA			21
#define	DELAY_WRITE_CODEC			91

#define	DELAY_RECORD_SAMPLING		0
#define	DELAY_READ_CODEC			91
#define	DELAY_ENCODE_DATA			101
#define DELAY_WRITE_SPI				162

#define	DELAY_MODE_SELECT	67
#define DELAY_CHANGE_CODEC_SAMPLING		11

/* FRAME_PERIOD is one less than the number of timer
 * ticks in each period for zero based index. The
 * number of timer ticks is equal to the number of
 * audio samples per frame.
 */
#define FRAME_PERIOD	79

/******* Function Prototypes *******/
void TASKS_audioCodecInit();
void TASKS_changeCodecSampling();
void TASKS_modeSelect();
void TASKS_encodeData();
void TASKS_readCodec();
void TASKS_decodeData();
void TASKS_writeCodec();
void TASKS_writeToSPI();
void TASKS_readFromSPI();

void TASKS_readSFM();
void TASKS_writeSFM();

#endif
/******* End of File *******/
