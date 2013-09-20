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

// Timer tick input pin
// The pin which receives the timer tick signal is P1.6
#define TIMER_TICK_INPUT_PIN (BIT6)

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
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK (0xAA)
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
