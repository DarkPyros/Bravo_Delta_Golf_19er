/*------------------------------------------------------------------*-
  hSCH.h (v1.00)
  ------------------------------------------------------------------

- see hSCH.C for details

-*------------------------------------------------------------------*/

#ifndef HSCH_H_
#define HSCH_H_

#include "../main.h"

// ------ Public data type declarations ----------------------------
// Total memory per task is 8 bytes
typedef struct
{
	// Pointer to the task (must be a ‘void (void)’ function)
	void (* pTask)(void);

	// Delay (ticks) until the function will (next) be run
	// - see SCH_Add_Task() for further details
	tWord Delay;

	// Interval (ticks) between subsequent runs.
	// - see SCH_Add_Task() for further details
	tWord Period;

	// Incremented (by scheduler) when task is due to execute
	tByte RunMe;

	// Set to 1 if task is co-operative
	// Set to 0 if task is pre-emptive
	tByte Co_op;

} sTaskH;

// ------ Public variable declarations -----------------------------

// The array of tasks (see hSCH.C)
extern sTaskH hSCH_tasks_G[hSCH_MAX_TASKS];

// The error code variable
//
// See Main.H for port on which error codes are displayed
// and for details of error codes
extern tByte Error_code_G;

#define CO_OP (1)
#define PRE_EMP (0)

// ------ Public function prototypes -------------------------------
// Core scheduler functions

void hSCH_Dispatch_Tasks(void);
tByte hSCH_Add_Task(void (*)(void), tWord, tWord, tByte);
tByte hSCH_Delete_Task(tByte);
void hSCH_Report_Status(void);
tByte hSCH_Set_Error_Code(tByte);
tByte hSCH_Get_Error_Code(void);

#endif /* HSCH_H_ */

/*------------------------------------------------------------------*-
---- END OF FILE ------------------------------------------------
-*------------------------------------------------------------------*/
