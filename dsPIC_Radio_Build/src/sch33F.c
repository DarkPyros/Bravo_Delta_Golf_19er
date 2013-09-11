#include "../h/includes.h"

extern sTask SCH_tasks[SCH_MAX_TASKS];

void SCH_initExtTrigger(){
	
	/* Configure Change Notification pin 7 */
	CNEN1bits.CN7IE = 1; // Enable CN7 pin for interrupt detection
	IEC1bits.CNIE = 1; // Enable CN interrupts
	IFS1bits.CNIF = 0; // Reset CN interrupt	
}

void SCH_update() {
	
	int i;
	
	for(i=0; i < SCH_MAX_TASKS; i++){
		if(SCH_tasks[i].pTask){
			/* If ExecitionTick == 255, the task is pre-emptive 
			 * and run immediately. */
			if(SCH_tasks[i].ExecutionTick == 255) {
				(*SCH_task[i].pFunction)();
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

int SCH_addTask(void (code* pFunction)(), const int tick){
	int i = 0;
	
	while( (SCH_tasks[i].pTask != 0) && (i < SCH_MAX_TASKS) ){
		i++;
	}
	
	if(i >= SCH_MAX_TASKS){
		return SCH_MAX_TASKS;
	}
	
	SCH_tasks[i].pTask = pFunction;
	SCH_tasks[i].ExecutionTick = tick;
	SCH_tasks[i].RunMe = 0;
	
	/* Return array position of task */
	return i;
}

unsigned char SCH_deleteTask(const int);
/* End of File */