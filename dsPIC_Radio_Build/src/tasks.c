#include "..\h\includes.h"

/******* Private data declarations *******/
/* PACKED_BYTES - The number of bytes contained in G726A
 * packed data array					
 */
#define PACKED_BYTES 20

/* Allocate state memory and IO buffers for G.726A encoder and decoder */
int rawSamples		[G726A_FRAME_SIZE]; 
int decodedSamples	[G726A_FRAME_SIZE];
unsigned char encodedSamples[G726A_FRAME_SIZE];
unsigned char packedData[PACKED_BYTES];
unsigned char encoder[G726A_ENCODER_SIZE];
unsigned char decoder[G726A_DECODER_SIZE];

#define SPEECH_SEGMENT_SIZE   		5000L 	//9728L	
char savedSpeechSegment[SPEECH_SEGMENT_SIZE];
long currentReadAddress = 0;
long writeAddress = 0;

/* Allocate memory for buffers and drivers
 * codecBuffer - Buffer used by the codec driver
 */
int 	codecBuffer		[WM8510DRV_DRV_BUFFER_SIZE];

WM8510Handle codec;
WM8510Handle * codecHandle = &codec;

/* taskID - Returned value from last SCH_addTask() call.
 * 			Used for SCH_deleteTask()
 * transceiverReady - Set FALSE to prevent SYNC_CLK
			output until CC430 is initialized.
 * modeFlag - Indicates if CC430 is placing dsPIC in 
 * 			record, playback or idle mode.
 * currentMode - Indicates the mode of hte dsPIC until 
 *			the next mode check with CC430.
 */
int taskID;
int transceiverReady = FALSE;

enum modes 	modeFlag;
enum modes	currentMode;

/******* Function Definitions *******/
/* Initialization of the WM8510 codec and G726A codec */
void TASKS_audioCodecInit() {
	
	/* Initialize the codec drivers	*/
	WM8510Init(codecHandle,codecBuffer);
	
	/* Start Audio input and output function */
	WM8510Start(codecHandle);
		
	/* Configure codec for 8K operation	*/
	WM8510SampleRate8KConfig(codecHandle);

	/* Initialize G.726A Encoder and Decoder. */
	G726AEncoderInit(encoder, G726A_16KBPS, G726A_FRAME_SIZE);
	G726ADecoderInit(decoder, G726A_16KBPS, G726A_FRAME_SIZE);

} /* End of audioCodecInit() */

/* Select the audio processing mode to run*/
void TASKS_modeSelect() {

	int i = 0;
	
	/* 28.4 microsecond timeout */
	TIMER_timer1Start(592);
//	TIMING_PULSE_PIN ^= 1;
//	while(!_T1IF);
//	TIMING_PULSE_PIN ^= 1;
//	TIMER_timer1Stop();
	
//if( (modeFlag == IDLE) || (modeFlag ==PLAYBACK) )
//	modeFlag = RECORD;
//else modeFlag = PLAYBACK;

	#if defined TIMING_MODE_SELECT
	TIMING_PULSE_PIN ^= 1;
	#endif
	
#if defined TIMING_TEST
	/* Check the switches for new operating mode */
	if( (CheckSwitchS1()) == 1 ) {
		modeFlag = PLAYBACK;
	}
	else if( (CheckSwitchS2()) == 1 ) {
		modeFlag = RECORD;
	}
#else
	/* Check the flags for new operating mode */
	if(PLAYBACK_FLAG == ACTIVE_MODE) {
		modeFlag = PLAYBACK;
	}
	else if(RECORD_FLAG == ACTIVE_MODE) {
		modeFlag = RECORD;
	}
	else {
		modeFlag = IDLE;
	}
#endif
	
	/* If operating mode has changed, delete existing tasks
	 * and add tasks for new mode. */
	if(modeFlag != currentMode) {
	
		if(modeFlag == PLAYBACK) {
			/* Toggle indicator LEDs. */		 
			YELLOW_LED = SASK_LED_OFF;
			GREEN_LED = SASK_LED_ON;
			
			/* Delete all tasks expect pre-emptive task [0]
			 * and modeSelect task [1]
			 */
			for(i=2; i < (SCH_MAX_TASKS); i++) {
				SCH_deleteTask(i);
			}

			//SCH_addTask(TASKS_readFromSPI, DELAY_READ_SPI, 	  FRAME_PERIOD, CO_OP);
			SCH_addTask(TASKS_playbackSpeechSegment, (DELAY_DECODE_DATA - 2), FRAME_PERIOD, CO_OP);
			SCH_addTask(TASKS_decodeData,  DELAY_DECODE_DATA, FRAME_PERIOD, CO_OP);
			SCH_addTask(TASKS_writeCodec,  DELAY_WRITE_CODEC, FRAME_PERIOD, CO_OP);
		}	
		else if(modeFlag == RECORD) {
			/* Toggle indicator LEDs. */		 
			YELLOW_LED = SASK_LED_ON;
			GREEN_LED = SASK_LED_OFF;
			
			/* Delete all tasks expect pre-emptive task [0]
			 * and modeSelect task [1]
			 */
			for(i=2; i < (SCH_MAX_TASKS); i++) {
				SCH_deleteTask(i);
			}
			
			SCH_addTask(TASKS_readCodec,  DELAY_READ_CODEC,  FRAME_PERIOD, CO_OP);
			SCH_addTask(TASKS_encodeData, DELAY_ENCODE_DATA, FRAME_PERIOD, CO_OP);
			//SCH_addTask(TASKS_writeToSPI, DELAY_WRITE_SPI,   FRAME_PERIOD, CO_OP);
			SCH_addTask(TASKS_recordSpeechSegment, (DELAY_ENCODE_DATA + 26), FRAME_PERIOD, CO_OP);
		}	
		else {
			/* Toggle indicator LEDs. */
			YELLOW_LED = SASK_LED_OFF;
			GREEN_LED = SASK_LED_OFF;
					
			/* Delete all tasks expect pre-emptive task [0]
			 * and modeSelect task [1]
			 */
			for(i=2; i < (SCH_MAX_TASKS); i++) {
				SCH_deleteTask(i);
			}
		}
		
		/* Add task to change the codec sampling mode at
		 * the end of the frame. Task does not repeat.*/
		SCH_addTask(TASKS_changeCodecSampling, DELAY_CHANGE_CODEC_SAMPLING, 0, CO_OP);
		
		/* Current mode is set to new mode*/
		currentMode = modeFlag;
	}
	/* If operating mode has not change, do nothing*/
	else {
	}

	/* Wait for timeout and disable/clear timer */
//	while(!_T1IF);
//	TIMER_timer1Stop();
	
	#if defined TIMING_MODE_SELECT
	TIMING_PULSE_PIN ^= 1;
	#endif
} /* End of modeSelect() */

/* Change the pre-emptive task based on a new operating mode */
void TASKS_changeCodecSampling() {
	
	#if defined TIMING_CHANGE_SAMPLING
	TIMING_PULSE_PIN ^= 1;
	#endif
	
	/* Delete current pre-emptive task from list */
	SCH_deleteTask(0);
	
	/* Add new pre-emptive based on current operating mode*/
	if(currentMode == PLAYBACK) {
		SCH_addTask(WM8510PlaybackSampling, DELAY_PLAYBACK_SAMPLING, 0, PRE_EMPTIVE);
	}
	else if(currentMode == RECORD) {
		SCH_addTask(WM8510RecordSampling, DELAY_RECORD_SAMPLING, 0, PRE_EMPTIVE);
	}
	/* currentMode is IDLE, so there is no pre-emptive task */
	else {
		SCH_addTask(WM8510IdleSampling, DELAY_PLAYBACK_SAMPLING, 0, PRE_EMPTIVE);	
	}

	#if defined TIMING_CHANGE_SAMPLING
	TIMING_PULSE_PIN ^= 1;
	#endif
} /* End of changeCodecSampling() */

/* Read new frame from codec and encode. */
void TASKS_encodeData() {
				
	int i;	

	#if defined TIMING_ENCODE_DATA
		TIMING_PULSE_PIN ^= 1;
	#endif

	/*Obtain Audio Samples
	while(WM8510IsReadBusy(codecHandle));
	*/
	
	/* Wait till the codec is ready with a new frame */	
	if(WM8510IsReadBusy(codecHandle) == FALSE) {
		
		/* Scale samples from 16-bit to 14-bit */
		for(i = 0; i < G726A_FRAME_SIZE; i ++)
		{
			/* Not necessary if its known that input is 14 bits or less. */	
			rawSamples[i] = rawSamples[i] >> 2;
		}

		/* Encode samples*/
		G726AEncode(encoder,rawSamples,encodedSamples);
		
		/* Compresses 80 encoded samples into 20 bytes of data prior to transmission */
		G726APack(encodedSamples, packedData, G726A_FRAME_SIZE, G726A_16KBPS);
		
	}

#if defined TIMING_ENCODE_DATA
	TIMING_PULSE_PIN ^= 1;
#endif
} /* End of encodeData() */

/* Obtain audio samples from codec */
void TASKS_readCodec() {
	#if defined TIMING_READ_CODEC
	TIMING_PULSE_PIN ^= 1;
	#endif

	WM8510Read(codecHandle, rawSamples, G726A_FRAME_SIZE);

	#if defined TIMING_READ_CODEC
	TIMING_PULSE_PIN ^= 1;
	#endif
} /* End of readCodec() */

/* Write decoded frame to codec for playback. */
void TASKS_decodeData() {

	int i;	
	
	#if defined TIMING_DECODE_DATA
		TIMING_PULSE_PIN ^= 1;
	#endif
	
	/* Wait till the codec is available for a new frame	
	while(WM8510IsWriteBusy(codecHandle));	
	*/
	
	/* Wait till the codec is available for a new frame */
	if(WM8510IsWriteBusy(codecHandle) == FALSE) {

#ifndef	NO_PACKING
		/* Uncompress 20 bytes of received data into 80 encoded samples */
		G726AUnpack(packedData, encodedSamples, G726A_FRAME_SIZE, G726A_16KBPS);
#endif

		/* Decode the samples*/
		G726ADecode(decoder, encodedSamples, decodedSamples);

		/* Scale sample*/
		for(i = 0; i < G726A_FRAME_SIZE; i ++) 	{
			/* Scale the decoded sample to 
			 * adjust for the 16 to 14-bit scaling done before
			 * encode */
			decodedSamples[i] = decodedSamples[i] << 2;
		}
	}
	else {
	}

	#if defined TIMING_DECODE_DATA
		TIMING_PULSE_PIN ^= 1;
	#endif
}	/* End of decodeData() */

/* Write the frame to the output. */
void TASKS_writeCodec() {
	#if defined TIMING_WRITE_CODEC
		TIMING_PULSE_PIN ^= 1;
	#endif

	WM8510Write(codecHandle, decodedSamples, G726A_FRAME_SIZE);

	#if defined TIMING_WRITE_CODEC
	TIMING_PULSE_PIN ^= 1;
	#endif
} /* End of writeCodec */

/* Transmit an array of data using SPI */
void TASKS_writeToSPI() {

	int SPITimeOut = 5000;
	
	#if defined TIMING_WRITE_SPI
	TIMING_PULSE_PIN ^= 1;
	#endif

	unsigned char currentByte = 0;
	
	TIMER_timer1Start(SPITimeOut);

	while(currentByte < PACKED_BYTES) {

		#ifndef NO_WAIT_FOR_SPI
		while(!SPI1STATbits.SPITBF  && !_T1IF);
		#endif		
		SPI1BUF = packedData[currentByte];
		
		currentByte++;
	}

	TIMER_timer1Stop();

	#if defined TIMING_WRITE_SPI
	TIMING_PULSE_PIN ^= 1;
	#endif
} /* End of writeToSPI() */

/* Receive an array of data using SPI*/
void TASKS_readFromSPI() {
	
	int SPITimeOut = 5000;
	
	#if defined TIMING_WRITE_SPI
	TIMING_PULSE_PIN ^= 1;
	#endif

	unsigned char currentByte = 0;
	
	TIMER_timer1Start(SPITimeOut);

	while(currentByte < PACKED_BYTES) {

		#ifndef NO_WAIT_FOR_SPI
		while(!SPI1STATbits.SPIRBF && !_T1IF);
		#endif		
		packedData[currentByte] = SPI1BUF;
		
		currentByte++;
	}
	
	TIMER_timer1Stop();

	#if defined TIMING_READ_SPI
	TIMING_PULSE_PIN ^= 1;
	#endif
} /* End of readFromSPI() */

void TASKS_playbackSpeechSegment() {
	#if defined TIMING_READ_SPEECH_SEGMENT
	TIMING_PULSE_PIN ^= 1;
	#endif
	
	int i;
	
	for(i=0; i < PACKED_BYTES; i++) {
		packedData[i] = savedSpeechSegment[currentReadAddress+i];
	}
	
	writeAddress += PACKED_BYTES;
	
	if(currentReadAddress >= SPEECH_SEGMENT_SIZE) {
		currentReadAddress = 0;
	}
	
	#if defined TIMING_READ_SPEECH_SEGMENT
	TIMING_PULSE_PIN ^= 1;
	#endif
} /* End of readSpeechSegment() */

void TASKS_recordSpeechSegment() {
	#if defined TIMING_WRITE_SPEECH_SEGMENT
	TIMING_PULSE_PIN ^= 1;
	#endif

	int i;
	
	for(i=0; i < PACKED_BYTES; i++) {
		savedSpeechSegment[writeAddress+i] = packedData[i]; 
	}
	
	writeAddress += PACKED_BYTES;
	
	if(writeAddress >= SPEECH_SEGMENT_SIZE) {
		writeAddress = 0;
	}
	
	#if defined TIMING_WRITE_SPEECH_SEGMENT
	TIMING_PULSE_PIN ^= 1;
	#endif
} /* End of TASKS_recordSpeechSegment() */
/******* End of File *******/
