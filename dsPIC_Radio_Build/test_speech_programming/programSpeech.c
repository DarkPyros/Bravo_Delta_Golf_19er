/* This program will program the Introductory speech segment into the
 * serial flash memory. The program starts and the green led will glow.
 * Pressing switch S1 will cause the program and verify operation to 
 * start. The yellow led will glow when this programming takes palce.
 * If the verify fails then red led will glow. If the verify is success
 * then green led will glow */


#include "..\h\p33FJ256GP506.h"
#include "..\h\sask.h"
#include "..\test_speech_programming\PgmMemory.h"
#include "..\test_speech_programming\programSpeech.h"

/* Program Memory Reader */
ProgramMemory PGMReader;

/* This is a pointer to the speech segment in the
 * program memory. Note how it is declared as a 
 * void function. The pointer to this function is 
 * the start address of the speech segment.
 * */

extern 	 void 	SpeechSegment();

char savedSpeechSegment[SPEECH_SEGMENT_SIZE];

void programSpeech (void) {

	/* PGMReaderHandle - Handle to the Program Memory Reader 
	 * bytesRead - tracks the number of bytes processed.
	 * dataByte - byte read from program flash
	 * */

	unsigned long	bytesRead;
	char			dataByte;
	ProgramMemory * PGMReaderHandle;


	/* Initialize the Program Memory Reader	*/
	PGMReaderHandle = PGMemoryOpen(&PGMReader,(long)SpeechSegment);

	/* Turn
	 * on the Green LED and then wait for the 
	 * S1 to be pressed. Pressing S1 will cause
	 * the programming to start. 
	 * */

	GREEN_LED = SASK_LED_ON;
	while(CheckSwitchS1() == 0);

	/* Turn on Yellow LED, turn off Green LED.
	 * Unprotect the flash.
	 * Erase the flash. Read the program memory
	 * and then program the serial flash memory
	 * */	

	GREEN_LED = SASK_LED_OFF;
	YELLOW_LED = SASK_LED_ON;
	
	/* Read the dsPIC33F program flash and program 
	 * serial flash. Read and write one byte at a time.  
	 * */
	
	bytesRead = 0;
	while (bytesRead < SPEECH_SEGMENT_SIZE)
	{
		PGMemoryRead(PGMReaderHandle,&(savedSpeechSegment[bytesRead]),1);
		bytesRead ++;
	}
	
	/* Programming is complete at this point. Now
	 * verify the programming operation. Re-open the
	 * PGMReader. This will reset the stream pointer
	 * to the start of the stream. Each byte from the
	 * serial flash memory is compared with the speech
	 * segment in the program memory. If the verify 
	 * fails, the Red LED is turned on and code will
	 * wait in an endless while loop. 
	 * */

	PGMReaderHandle = PGMemoryOpen(&PGMReader,(long)SpeechSegment);
	bytesRead = 0;

	while (bytesRead < SPEECH_SEGMENT_SIZE)
	{
		PGMemoryRead(PGMReaderHandle,&dataByte,1);
		if(savedSpeechSegment[bytesRead] != dataByte)
		{
			RED_LED = SASK_LED_ON;
			YELLOW_LED = SASK_LED_OFF;
			while(CheckSwitchS1() == 0);
			YELLOW_LED = SASK_LED_ON;
		}
		bytesRead ++;
	}

	/* At this point the program and verify operation
	 * was successful. Switch on the Green LED and wait
	 * until SW1 press.
	 * */

	YELLOW_LED = SASK_LED_OFF;
	GREEN_LED = SASK_LED_ON;
	while(CheckSwitchS1() == 0);
	GREEN_LED = SASK_LED_OFF;


} /* End of programSpeech() */
