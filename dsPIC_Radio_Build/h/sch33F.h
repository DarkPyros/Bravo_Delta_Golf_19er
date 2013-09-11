#ifndef sch_H
#define	sch_H

#define	SCH_update()	{__attribute__((__interrupt__,no_auto_psv)) _CNInterrupt()}

/******* Public data type declarations *******/

/* Store in DATA area, if possible, for rapid access 
 * Total memory per task is 7 bytes
 */
typedef data struct
{
	// Pointer to the task (must be a 'void (void)' function)
	void (code * pTask)(void); 
	// The tick the task will be run on
	int ExecutionTick;
	// Indicates if task is ready to execute
	int RunMe;
} sTask;

/******* Function Prototypes *******/
	
void SCH_initExtTrigger();
void SCH_dispatchTasks();
int SCH_addTask(void (code*)(), const int, const int);
unsigned char SCH_deleteTask(const int);
void SCH_update();


/******* Public Contstants *******/
/* The maximum number of tasks required at any one time
 * during the execution of the program
 *
 * MUST BE ADJUSTED FOR EACH NEW PROJECT
 */
#define SCH_MAX_TASKS (2)

#endif