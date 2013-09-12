#include "../h/includes.h"

sTask SCH_tasks[SCH_MAX_TASKS];

int tickCount = 0;

void SCH_initExtTrigger(){
	
	/* Initialize PortB pin as output for synchronization clock pulse */
	SYNC_CLK_PULSE_TRIS	= 0;
	SYNC_CLK_PULSE_PIN	= 0;		
}

void SCH_update() {
	
	/* Send synchronization clock pulse to other micro-controller*/
	SYNC_CLK_PULSE_PIN ^= 1;
	
	/* Increment the frame time counter*/
	if(tickCount == SCH_MAX_TICKS) {
		tickCount = 0;
	}
	else {
		tickCount++;
	}

	int i;
	
	for(i=0; i < SCH_MAX_TASKS; i++){
		if(SCH_tasks[i].pTask){
			/* If ExecitionTick == 255, the task is pre-emptive 
			 * and run immediately. */
			if(SCH_tasks[i].ExecutionTick == 255) {
				(*(SCH_task[i]).pFunction)();
			}
			
			else if(SCH_tasks[i].ExecutionTick == tickCount) {
				SCH_tasks[i].RunMe = 1;	/* Set RunMe flag */
			}			
		}
	}
}

void SCH_dispatchTasks() {
	int i = 0;
	
	for(i=0; i < SCH_MAX_TASKS; i++) {
		if(SCH_tasks[i].RunMe) {
			(*SCH_task[i].pFunction)();
			
			SCH_tasks[i].RunMe = 0;
		}
	}
}

/* tick = the sample tick during a frame the task 
 * will execute. Value between 1 to SCH_MAX_TICKS
 */ 
int SCH_addTask(void (*pFunction)(), const int tick){
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
		SCH_tasks[i].RunMe = 0;
	}	
	
	/* Return array position of task */
	return i;
}

unsigned char SCH_deleteTask(const int) {

}
/* End of File */