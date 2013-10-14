/*------------------------------------------------------------------*-
  Tasks.H (v1.00)
  ------------------------------------------------------------------

  Task list header file

-*------------------------------------------------------------------*/

#ifndef TASKS_H_
#define TASKS_H_

#include "main.h"

#define RADIO_CHANGE_CHANNEL_DELAY	(0)
#define RNG_GET_RAND_TASK_DELAY		(1)
#define START_SYNC_TASK_DELAY		(2)
#define ENCRYPT_DATA_TASK_DELAY		(5)
#define UART_SEND_DATA_TASK_DELAY	(6)
#define TASK_MANAGER_DELAY			(79)

typedef enum { RECEIVE, TRANSMIT } MODE;
typedef enum { MASTER, SLAVE } ROLE;
typedef enum { SYNCED, SYNC_LOST } SYNC;

void Schedule_Tasks (void);
void Change_Channel_Task (void);
void Task_Manager (void);
void Start_Synchronization_Task (void);
void Slave_Synchronization_Task (void);
void Get_Next_Channel_Task (void);
void Encrypt_Data_Task (void);
void SPI_Send_Task (void);
void SPI_Receive_Task (void);
void UART_Send_Task (void);

#endif /* TASKS_H_ */
