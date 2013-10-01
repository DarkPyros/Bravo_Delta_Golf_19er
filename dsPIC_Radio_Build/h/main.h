#ifndef main_H
#define main_H

#define TRUE 1
#define FALSE 0

/* Uncomment to user SFM chip */
//#define 	USE_SFM_CHIP
/* Uncomment to remove SPI buffer while loop */
//#define 	NO_WAIT_FOR_SPI
/* Uncomment to remove Packing and Unpacking of
 * encoded samples
 */
//#define 	NO_PACKING

#define 	TIMING_TEST
//#define 	TIMING_SCH_UPDATE
//#define 	TIMING_MODE_SELECT
#define 	TIMING_ENCODE_DATA
#define 	TIMING_DECODE_DATA
//#define	TIMING_WRITE_SPI
//#define 	TIMING_READ_SPI
//#define 	TIMING_READ_CODEC
//#define 	TIMING_WRITE_CODEC
//#define 	TIMING_CHANGE_SAMPLING

//#define 	TIMING_READ_SPEECH_SEGMENT
//#define 	TIMING_WRITE_SPEECH_SEGMENT

#endif
