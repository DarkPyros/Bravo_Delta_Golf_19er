/*------------------------------------------------------------------*-
  Main.H (v1.00)
  ------------------------------------------------------------------

  Project Header for project Freq_Hop_Radio

-*------------------------------------------------------------------*/

#ifndef MAIN_H_
#define MAIN_H_

//----------------------------------------------------------------------
// WILL NEED TO EDIT THIS SECTION FOR EVERY PROJECT
//----------------------------------------------------------------------

// Must include the appropriate microcontroller header file here
#include <msp430.h>
#include "./HAL/HAL_MACROS.h"
#include "./HAL/HAL_PMM.h"

// Must include oscillator / chip details here if delays are used
// -
// Oscillator frequency (in Hz) e.g. (19500000UL)
#define OSC_FREQ (19500000UL)

// ------ Public constants -----------------------------------------

// The maximum number of tasks required at any one time
// during the execution of the program
//
// MUST BE ADJUSTED FOR EACH NEW PROJECT
#define hSCH_MAX_TASKS (10)

// Timer tick rate in the number of microseconds between
// timer ticks. This is used primarily for sandwich
// timer delays.
#define TIMER_TICK_RATE (125)

// Number of timer ticks per 10 ms frequency hop. This is
// also the number of ticks per 10 ms audio frame.
#define TICKS_PER_FRAME (80)

// Timer tick input pin
// The pin which receives the timer tick signal is P1.6
#define TIMER_TICK_INPUT_PIN (BIT6)

// Audio frame size in bytes
#define AUDIO_FRAME_SIZE (20)

// UART debugging defines
//#define TEXT_BINARY
#define TEXT_HEX
//#define DEBUG

// Defines for setting the flag output port and pins
// for signaling the dsPIC as to what to do next.
#define RECORD_FLAG (BIT0)
#define PLAYBACK_FLAG (BIT2)
#define FLAG_PORT_DIR P3DIR
#define FLAG_PORT_SEL P3SEL
#define FLAG_PORT P3OUT

// Defines for using the LED on P1.0 for debugging
#define LED (BIT0)
#define LED_ON P1OUT |= LED
#define LED_OFF P1OUT &= ~LED
#define LED_PORT_DIR P1DIR
#define LED_PORT_SEL P1SEL

// Defines for using the LED on P3.6 for debugging
#define RED_LED (BIT6)
#define RED_LED_ON P3OUT |= RED_LED
#define RED_LED_OFF P3OUT &= ~RED_LED
#define RED_LED_PORT_DIR P3DIR
#define RED_LED_PORT_SEL P3SEL

// ------ hSCH.C ----------------------------------------

// Comment this line out if error reporting is NOT required
//#define SCH_REPORT_ERRORS

#ifdef SCH_REPORT_ERRORS
// The port on which error codes will be displayed
// ONLY USED IF ERRORS ARE REPORTED
#define Error_port P1OUT

#endif

//----------------------------------------------------------------------
// SHOULD NOT NEED TO EDIT THE SECTIONS BELOW
//----------------------------------------------------------------------
typedef unsigned char tByte;
typedef unsigned int  tWord;
typedef unsigned long tLong;
typedef unsigned long long tLongLong;

// Misc #defines
#ifndef TRUE
#define FALSE 0
#define TRUE (!FALSE)
#endif

#define RETURN_NORMAL (0)
#define RETURN_ERROR  (1)

// ---------------------------------------------------------------------
// Error codes
//----------------------------------------------------------------------
#define ERROR_SCH_TOO_MANY_TASKS (1)
#define ERROR_SCH_CANNOT_DELETE_TASK (2)
#define ERROR_SCH_SPI_TX_ERROR (3)
#define ERROR_SCH_SPI_RX_ERROR (4)
#define ERROR_RADIO_RX_CRC_BAD (5)

#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER (0xAA)
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START (0xA0)
#define ERROR_SCH_LOST_SLAVE (0x80)
#define ERROR_I2C_WRITE_BYTE_AT24C64 (11)
#define ERROR_I2C_READ_BYTE_AT24C64 (12)
#define ERROR_I2C_WRITE_BYTE (13)
#define ERROR_I2C_READ_BYTE (14)
#define ERROR_USART_TI (21)
#define ERROR_USART_WRITE_CHAR (22)

//======================================================================

#endif /* MAIN_H_ */
