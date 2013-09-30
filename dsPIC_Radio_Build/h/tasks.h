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
/* Task delay times*/
#define	DELAY_PLAYBACK_SAMPLING		0
#define	DELAY_READ_SPI				69
#define DELAY_DECODE_DATA			30
#define	DELAY_WRITE_CODEC			77

#define	DELAY_RECORD_SAMPLING		0
#define	DELAY_READ_CODEC			159
#define	DELAY_ENCODE_DATA			160
#define DELAY_WRITE_SPI				161

#define	DELAY_MODE_SELECT	0
#define DELAY_CHANGE_CODEC_SAMPLING		78

/* The number of timer ticks in each period.
 * This value is equal to the number of audio samples.
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

void TASKS_playbackSpeechSegment();
void TASKS_recordSpeechSegment();

#endif
/******* End of File *******/
