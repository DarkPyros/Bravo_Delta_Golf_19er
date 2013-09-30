#ifndef	PROGRAM_SPEECH_H
#define	PROGRAM_SPEECH_H

/* SPEECH_SEGMENT_SIZE - Number of elements in the program memory speech
 * segment array.
 * */
#define SPEECH_SEGMENT_SIZE   		14000L 	//9728L	

extern char savedSpeechSegment[SPEECH_SEGMENT_SIZE];

void programSpeech (void);

#endif
