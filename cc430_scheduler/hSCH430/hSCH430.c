/*------------------------------------------------------------------*-

  hSCH430.C (v1.00)

  ------------------------------------------------------------------

  *** THIS IS A *HYBRID* SCHEDULER FOR TI CC430 ***
  *** Uses external interrupt on P1.1 for timing ***
  *** 125 us (precise) tick interval ***
  ***
-*------------------------------------------------------------------*/

#include "hSCH430.h"

// ------ Public variable declarations -----------------------------

// The array of tasks (see hSCH.C)
extern sTaskH hSCH_tasks_G[hSCH_MAX_TASKS];

// The error code variable
//
// See Main.H for port on which error codes are displayed
// and for details of error codes
extern tByte Error_code_G;

/*------------------------------------------------------------------*-

  hSCH_Init_P1_1()

  Scheduler initialization function. Prepares scheduler
  data structures and sets up to receive timer interrupt on P1.1.
  Must call this function before using the scheduler.

-*------------------------------------------------------------------*/
void hSCH_Init_P1_1(void)
{
	tByte i;

	for (i = 0; i < hSCH_MAX_TASKS; i++)
	{
		hSCH_Delete_Task(i);
	}

	// Reset the global error variable
	// - hSCH_Delete_Task() will generate an error code,
	// (because the task array is empty)
	Error_code_G = 0;

	// Set up P1.1 as the timer tick interrupt source
	// P1SEL disables the P1.1 peripheral function
	// P1DIR sets P1.1 as an input
	// P1REN enables the pull-up/down resistor
	// P1IES sets a high-to-low transition interrupt edge select
	// P1IFG clears any pending interrupts
	// P1OUT sets the pull resistor as a pull-up
	// P1IE enables the interrupt
	P1SEL &= ~BIT1;
	P1DIR &= ~BIT1;
	P1REN |= BIT1;
	P1OUT |= BIT1;
	P1IE  |= BIT1;
	P1IES |= BIT1;
	P1IFG &= ~BIT1;
}

/*------------------------------------------------------------------*-

  hSCH_Start()

  Starts the scheduler, by enabling interrupts.

  NOTE: Usually called after all regular tasks are added,
  to keep the tasks synchronized.

  NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!!

-*------------------------------------------------------------------*/
void hSCH_Start(void)
{
	__bis_SR_register( GIE );       // Enable interrupts
	__no_operation();               // For debugger
}

/*------------------------------------------------------------------*-

  hSCH_Update

  This is the scheduler ISR. It is called at a rate
  determined by the timer settings in hSCH_Init().
  This version is triggered by an external interrupt on P1.1.

-*------------------------------------------------------------------*/
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void hSCH_Update(void)
{
	tByte Index;

	// NOTE: calculations are in *TICKS* (not milliseconds)
	for (Index = 0; Index < hSCH_MAX_TASKS; Index++)
	{
		// Check if there is a task at this location
		if (hSCH_tasks_G[Index].pTask)
		{
			if (hSCH_tasks_G[Index].Delay == 0)
			{
				// The task is due to run
				//
				if (hSCH_tasks_G[Index].Co_op)
				{
					// If it is a co-operative task, increment the RunMe flag
					hSCH_tasks_G[Index].RunMe += 1;
				}
				else
				{
					// If it is a pre-emptive task, run it IMMEDIATELY
					(*hSCH_tasks_G[Index].pTask)(); // Run the task

					hSCH_tasks_G[Index].RunMe -= 1; // Reset / reduce RunMe flag

					// Periodic tasks will automatically run again
					// - if this is a ‘one shot’ task, remove it from the array
					if (hSCH_tasks_G[Index].Period == 0)
					{
						hSCH_tasks_G[Index].pTask = 0;
					}
				}

				if (hSCH_tasks_G[Index].Period)
				{
					// Schedule regular tasks to run again
					hSCH_tasks_G[Index].Delay = hSCH_tasks_G[Index].Period;
				}
			}
			else
			{
				// Not yet ready to run: just decrement the delay
				hSCH_tasks_G[Index].Delay -= 1;
			}
		}
	}

	P1IES ^= BIT1;  // toggle the interrupt edge,
			        // the interrupt vector will be called
			        // when P1.1 goes from High-to-Low as well as
			        // Low-to-High

	P1IFG &= ~BIT1; // Clear interrupt flag
}

/*------------------------------------------------------------------*-
---- END OF FILE ------------------------------------------------
-*------------------------------------------------------------------*/
