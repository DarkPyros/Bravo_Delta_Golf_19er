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
#define DECRYPT_DATA_TASK_DELAY		(1)
#define START_SYNC_TASK_DELAY		(2)
#define SPI_SEND_TASK_DELAY			(4)
#define UART_SEND_DATA_TASK_DELAY	(6)
#define RECEIVING_MODE_TASK_DELAY	(8)
#define POLL_TRANSMIT_TASK_DELAY	(20)
#define CHECK_RX_DATA_TASK_DELAY	(46)
#define TASK_MANAGER_DELAY			(79)
#define SPI_RECEIVE_TASK_DELAY		(210)
#define	ENCRYPT_DATA_TASK_DELAY		(246)
#define	TRANSMIT_DATA_TASK_DELAY	(268)

typedef enum { RECEIVE, TRANSMIT } MODE;
typedef enum { MASTER, SLAVE } ROLE;
typedef enum { SYNCED, SYNC_LOST } SYNC;

void Task_Zero (void);
void Schedule_Tasks (void);
void Sync_Schedule_Tasks (void);
void Change_Channel_Task (void);
void Task_Manager (void);
void Start_Synchronization_Task (void);
void Slave_Synchronization_Task (void);
void Get_Next_Channel_Task (void);
void Encrypt_Data_Task (void);
void SPI_Send_Task (void);
void SPI_Receive_Task (void);
void UART_Send_Task (void);
void Poll_Transmit_Button_Task (void);
void Transmit_Data_Task (void);
void Receiving_Mode_Task (void);
void Check_RX_Data_Task (void);
void Decrypt_Data_Task (void);

#endif /* TASKS_H_ */
