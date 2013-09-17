#include "../h/includes.h"

#define	SCH_UPDATE()	__attribute__((__interrupt__,no_auto_psv))

sTask SCH_tasks[SCH_MAX_TASKS];

int tickCount = 0;

void SCH_initExtTrigger(){
	
	/* Initialize PortB pin as output for synchronization clock pulse */
	SYNC_CLK_PULSE_TRIS	= 0;
	SYNC_CLK_PULSE_PIN	= 0;		
}

void SCH_UPDATE() _DCIInterrupt() {
	
	/* Send synchronization clock pulse to other micro-controller*/
	SYNC_CLK_PULSE_PIN ^= 1;
	
	/* Increment the frame time counter*/
	if(tickCount == SCH_MAX_TICKS) {
		tickCount = 1;
	}
	else {
		tickCount++;
	}

	int i;
	
	for(i=0; i < SCH_MAX_TASKS; i++){
		if(SCH_tasks[i].pTask){
			/* If Co_op == 1, the task is co-operative 
			 * and run immediately. */
			if(SCH_tasks[i].Co_op == 1) {
				SCH_tasks[i].RunMe = 1;	/* Set RunMe flag */
			}
			/* If Co_op == 0, the task is pre-emptive 
			 * and runs immediately. */
			else {
				(SCH_tasks[i].pTask)();
			}			
		}
	}
}

void SCH_dispatchTasks() {
	int i = 0;
	
	for(i=0; i < SCH_MAX_TASKS; i++) {
		if(SCH_tasks[i].RunMe) {
			(SCH_tasks[i].pTask)();
			
			SCH_tasks[i].RunMe = 0;
		}
	}
}

/* tick = the sample tick during a frame the task 
 * will execute. Value between 1 to SCH_MAX_TICKS
 */ 
int SCH_addTask(void (*pFunction)(), const int tick, const int co_op){
	int i = 0;
	
	while( (SCH_tasks[i].pTask != 0) && (i < SCH_MAX_TASKS) ){
		i++;
	}
	
	if(i >= SCH_MAX_TASKS){
		return SCH_MAX_TASKS;
	}
	
	if( (tick >= 1) && (tick <= SCH_MAX_TICKS) ) {
		SCH_tasks[i].pTask = pFunction;
		SCH_tasks[i].ExecutionTick = tick;
		SCH_tasks[i].Co_op = co_op;
		SCH_tasks[i].RunMe = 0;
	}	
	
	/* Return array position of task */
	return i;
}

unsigned char SCH_deleteTask(const int taskIndex) {
	
	unsigned char returnCode;

	return returnCode;
}
/* End of File */
