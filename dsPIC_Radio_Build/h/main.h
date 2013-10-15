#ifndef main_H
#define main_H

#define TRUE 1
#define FALSE 0

/* Uncomment to user SFM chip */
#define 	USE_SFM_CHIP
/* Uncomment to remove SPI buffer while loop */
//#define 	NO_WAIT_FOR_SPI
/* Uncomment to enable LED error reporting */
//#define 	SCH_REPORT_ERRORS

#define 	STANDALONE_TEST
//#define 	TIMING_TEST
//#define 	TIMING_SCH_UPDATE
//#define 	TIMING_MODE_SELECT
//#define 	TIMING_ENCODE_DATA
//#define 	TIMING_DECODE_DATA
//#define	TIMING_WRITE_SPI
//#define 	TIMING_READ_SPI
//#define 	TIMING_READ_CODEC
//#define 	TIMING_WRITE_CODEC
//#define 	TIMING_CHANGE_SAMPLING

//#define 	TIMING_READ_SPEECH_SEGMENT
//#define 	TIMING_WRITE_SPEECH_SEGMENT

#if defined USE_SFM_CHIP

#include "..\h\SST25VF040BDrv.h"

#define FRAME_SIZE 				80		
#define SPEECH_SEGMENT_SIZE		29184L	
#define WRITE_START_ADDRESS	0x20000

extern char 	flashMemoryBuffer	[];
//SST25VF040BHandle flashMemoryHandle; 

/* Addresses 
 * currentReadAddress - This one tracks the intro message	
 * currentWriteAddress - This one tracks the writes to flash	
 * userPlaybackAddress - This one tracks user playback		
 * address - Used during flash erase
 * */

/*USE_SFM_CHIP */
extern long currentReadAddress;		
extern long currentWriteAddress;		
extern long userPlaybackAddress;		
extern long address;	
extern int erasedBeforeRecord;

void writeSFM();
void readSFM();
#endif


#endif
