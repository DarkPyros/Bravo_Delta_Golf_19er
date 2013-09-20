#ifndef sch_H
#define	sch_H

/******* Public data type declarations *******/

/* Store in DATA area, if possible, for rapid access 
 * Total memory per task is 7 bytes
 */
typedef struct tagsTask
{
	/* Pointer to the task (must be a 'void (void)' function) */
	void (*pTask)(void); 
	
	/* The tick the task will be run on */
	int ExecutionTick;
	
	/* Set to 1 if task is co-operative
	 * Set to 0 if task is pre-emptive
	 */
	int Co_op;

	/* Indicates if task is ready to execute */
	int RunMe;	
} sTask;

/******* Function Prototypes *******/
	
void SCH_initExtTrigger();
void SCH_dispatchTasks();
int SCH_addTask(void (*)(), const int, const int);
unsigned char SCH_deleteTask(const int);
void SCH_update();


/******* Public Constants *******/
/* The maximum number of tasks required at any one time
 * during the execution of the program
 *
 * MUST BE ADJUSTED FOR EACH NEW PROJECT
 */
#define SCH_MAX_TASKS (2)

/* The number of timer ticks in each period.
 * This value is equal to the number of audio samples.
 */
#define SCH_MAX_TICKS	80

#endif
