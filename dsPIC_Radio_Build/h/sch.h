#ifndef sch_H
#define	sch_H

/******* Public data type declarations *******/

/* Store in DATA area, if possible, for rapid access 
 * Total memory per task is 7 bytes
 */
typedef struct tagsTask
{
	/* Pointer to the task (must be a 'void (void)' function) */
	void (* pTask)(void); 
	/* Delay (ticks) until the function will (next) be run
	 * see SCH_Add_Task() for further details
	 */
	int Delay; 
	/* Interval (ticks) between subsequent runs.
	 * see SCH_Add_Task() for further details
	 */
	int Period;
	/* Set to 1 if task is co-operative
	 * Set to 0 if task is pre-emptive
	 */
	int Co_op;
	/* Incremented (by scheduler) when task is due to execute */
	int RunMe;
	
} sTask;

/******* Function Prototypes *******/
	
void SCH_initExtTrigger();
void SCH_dispatchTasks();
int  SCH_addTask(void (*)(), int, int, int);
void SCH_deleteTask(int);
void SCH_update();


/******* Public Constants *******/
/* The maximum number of tasks required at any one time
 * during the execution of the program
 *
 * MUST BE ADJUSTED FOR EACH NEW PROJECT
 */
#define SCH_MAX_TASKS 	6

/* Define task type for hybrid scheduler */
#define PRE_EMPTIVE		0
#define CO_OP			1

#endif
