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
			if(SCH_tasks[i].Delay == 0){
				SCH_tasks[i].RunMe = 1;	/* Set RunMe flag */
			}
			
			if(SCH_tasks[i].Period){
				SCH_tasks[i].Delay = SCH_tasks[i].Period;
			}
		}
		else{
			SCH_tasks[i].Delay -= 1;
		}
	}
}

void SCH_dispatchTasks()

int SCH_addTask(void (code* pFunction)(), const int DELAY, const int PERIOD){
	int i = 0;
	
	while( (SCH_tasks[i].pTask != 0) && (i < SCH_MAX_TASKS) ){
		i++;
	}
	
	if(i = SCH_MAX_TASKS){
		return SCH_MAX_TASKS;
	}
	
	SCH_tasks[i].pTask = pFunction;
	SCH_tasks[i].Delay = DELAY;
	SCH_tasks[i].Period = PERIOD;
	SCH_tasks[i].RunMe = 0;
	
	/* Return array position of task */
	return i;
}

unsigned char SCH_deleteTask(const int);
/* End of File */