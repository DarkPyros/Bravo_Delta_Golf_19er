/* Hybrid Scheduler */

#include "../h/includes.h"

#define	SCH_UPDATE()	__attribute__((__interrupt__,no_auto_psv))

#define ERROR_PORT	_LATC

sTask SCH_tasks[SCH_MAX_TASKS];

int	radioSyncingFlag = FALSE;

void SCH_initExtTrigger() {
#ifndef USE_SFM_CHIP	
	/* Initialize PortB pin as output for synchronization clock pulse */
	SYNC_CLK_PULSE_TRIS	= 0;
	SYNC_CLK_PULSE_PIN	= 0;
#endif
} /* End of SCH_initExtTrigger() */

void SCH_UPDATE() _DCIInterrupt() {

	_DCIIF = 0;
	
	int i;
		
	/* Start sending synchronization clock pulse to other micro-controller
	 * and updating tasks once the ready signal has been received*/
	if(transceiverReady == TRUE) {

#ifndef USE_SFM_CHIP
		SYNC_CLK_PULSE_PIN ^= 1;
#endif

#ifndef	STANDALONE_TEST
		SCH_checkRadioSyncingFlag();
#endif
		/* If CC430 is in synchronization mode the dsPIC does nothing */
		if(!radioSyncingFlag) {
			#if defined TIMING_SCH_UPDATE
	//		static int tickCounter = 0;
			
	//		if(tickCounter == 0) {
				TIMING_PULSE_PIN ^= 1;
	//		}
			#endif

			for(i=0; i < SCH_MAX_TASKS; i++){
				
				/* Check for task at this position */
				if(SCH_tasks[i].pTask){
					
					/* Check if task is due to run */
					if(SCH_tasks[i].Delay == 0) {
					
						/* If Co_op == 1, the task is co-operative 
						 * so set RunMe flag. */
						if(SCH_tasks[i].Co_op == 1) {
							SCH_tasks[i].RunMe = 1;
							
							/* Schedule periodic tasks to run again */
							if(SCH_tasks[i].Period) {
								SCH_tasks[i].Delay = SCH_tasks[i].Period;
							}
							else {
							}
						}
						/* If Co_op == 0, the task is pre-emptive 
						 * and runs immediately. */
						else {
							(SCH_tasks[i].pTask)();
							SCH_tasks[i].RunMe = 0;
						}				
					}
					else {
						/* Task not ready; decrement Delay */
						SCH_tasks[i].Delay -= 1;
					}		
				}
				else {
				}
			}
			
			#if defined TIMING_SCH_UPDATE
	//		if(tickCounter == 0) {
				TIMING_PULSE_PIN ^= 1;
	/*		}
			if(tickCounter >= 79) {
				tickCounter = 0;
			}
			else {
				tickCounter++;
			}
	*/		#endif
		}
		/* If radioSyncingFlaf is true, do nothing*/
		else {
		}
	}
	/* If transceiver is not ready, do nothing */
	else {
	}
} /* End of SCH_update() */

void SCH_dispatchTasks() {
	int i = 0;
	
	for(i=0; i < SCH_MAX_TASKS; i++) {
		if(SCH_tasks[i].RunMe) {
			(SCH_tasks[i].pTask)();
			
			/* If 'one shot' task, remove from array; otherwise make
			 * task not ready.
			 */
			if(SCH_tasks[i].Period == 0) {
				//SCH_tasks[i].pTask = 0;
				SCH_deleteTask(i);
			}
			else {
			SCH_tasks[i].RunMe = 0;
			}
		}
		else {
		}
	}
	
	/* Scheduler enters idle mode */
	SCH_goToSleep();	
} /* SCH_dispatchTasks() */

int SCH_addTask(void (*pFunction)(), int delay, int period, int co_op) {
	int i = 0;
	
	while( (SCH_tasks[i].pTask != 0) && (i < SCH_MAX_TASKS) ){
		i++;
	}
	
	if(i == SCH_MAX_TASKS) {
		return SCH_MAX_TASKS;
	}
	else {
		SCH_tasks[i].pTask = pFunction;
		SCH_tasks[i].Delay = delay;
		SCH_tasks[i].Period = period;
		SCH_tasks[i].Co_op = co_op;
		SCH_tasks[i].RunMe = 0;
		
		/* Return array position of task */
		return i;
	}
} /* SCH_addTask() */

void SCH_deleteTask(int task_index) {
	
	if(SCH_tasks[task_index].pTask) {
		SCH_tasks[task_index].pTask = 0x0000;
		SCH_tasks[task_index].Delay = 0;
		SCH_tasks[task_index].Period = 0;
		SCH_tasks[task_index].Co_op = 0;
		SCH_tasks[task_index].RunMe = 0;
	}
	else {
	}
} /* SCH_deleteTask() */

void SCH_start() {
	/* The scheduler interrupt is running from reset to allow communication
	 * with WM8510. Start the scheduler program by setting transceiverRead 
	 * to true */
	transceiverReady = TRUE;
} /* End of SCH_start() */

void SCH_goToSleep() {
	/* Macro for placing dsPIC CPU in idle mode */
//	Idle();
} /* End of SCH_goToSleep() */

void SCH_reportStatus() {
	#ifdef SCH_REPORT_ERRORS
	/* ONLY APPLIES IF WE ARE REPORTING ERRORS */
	
	/* Check for a new error code */
	if (SCH_errorCode != SCH_lastErrorCode) {
		// Negative logic on LEDs assumed
		Error_port = 255 - Error_code_G;
		Last_error_code_G = Error_code_G;
		
		if (Error_code_G != 0) {
		Error_tick_count_G = 60000;
		}
		else {
		Error_tick_count_G = 0;
		}
	}
	else {
		if (Error_tick_count_G != 0) {
			if (--Error_tick_count_G == 0) {
			Error_code_G = 0; // Reset error code
			}
		}
	}
	#endif
}

void SCH_checkRadioSyncingFlag() {
	int i;

	/* Check to see if CC430 is currently in synchronization mode. */
	if(START_FLAG_PIN == 0) {
		/* If CC430 is syncing, set radio sync flag and delete all tasks.*/
		radioSyncingFlag = TRUE;
		
		for(i=0; i < SCH_MAX_TASKS; i++) {
			SCH_deleteTask(i);
		}
	}
	else {
		/* CC430 is now synchronized. Add sampling and mode select tasks
		 * Unset radio sync flag to begin operating again.*/
		if(radioSyncingFlag == TRUE) {
			SCH_addTask(WM8510IdleSampling, DELAY_PLAYBACK_SAMPLING, 0, PRE_EMPTIVE);
			SCH_addTask(TASKS_modeSelect, (DELAY_MODE_SELECT + 1), FRAME_PERIOD, CO_OP);
			
			radioSyncingFlag = FALSE;
		}
		else {
		}
	}
}
/******* End of File *******/
