/* Hybrid Scheduler */

#include "../h/includes.h"

#define	SCH_UPDATE()	__attribute__((__interrupt__,no_auto_psv))

sTask SCH_tasks[SCH_MAX_TASKS];

void SCH_initExtTrigger() {
	
	/* Initialize PortB pin as output for synchronization clock pulse */
	SYNC_CLK_PULSE_TRIS	= 0;
	SYNC_CLK_PULSE_PIN	= 0;	
} /* End of SCH_initExtTrigger() */

void SCH_UPDATE() _DCIInterrupt() {
	
	/* Send synchronization clock pulse to other micro-controller */
	SYNC_CLK_PULSE_PIN ^= 1;
	
	int i;
	
	for(i=0; i < SCH_MAX_TASKS; i++){
		
		/* Check for task at this position */
		if(SCH_tasks[i].pTask){
			
			/* Check if task is due to run */
			if(SCH_tasks[i].Delay == 0) {
			
				/* If Co_op == 1, the task is co-operative 
				 * so set RunMe flag. */
				if(SCH_tasks[i].Co_op == 1) {
					SCH_tasks[i].RunMe = 1;
					
					/* Schedule periodic task to run again */
					if(SCH_tasks[i].Period) {
						SCH_tasks[i].Delay = SCH_tasks[i].Period;
					}
					/* If 'one shot' task, remove from array */
					else {
						SCH_tasks[i].pTask = 0;
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
	}
} /* SCH_update() */

void SCH_dispatchTasks() {
	int i = 0;
	
	for(i=0; i < SCH_MAX_TASKS; i++) {
		if(SCH_tasks[i].RunMe) {
			(SCH_tasks[i].pTask)();
			
			SCH_tasks[i].RunMe = 0;
		}
	}
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
} /* SCH_deleteTask() */

/******* End of File *******/
