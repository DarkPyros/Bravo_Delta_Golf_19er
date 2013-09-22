/*------------------------------------------------------------------*-
  Tasks.H (v1.00)
  ------------------------------------------------------------------

  Task list header file

-*------------------------------------------------------------------*/

#ifndef TASKS_H_
#define TASKS_H_

#include "main.h"

#define AUDIO_FRAME_SIZE (20)

typedef enum { RECEIVE, TRANSMIT } MODE;
typedef enum { MASTER, SLAVE } ROLE;
typedef enum { SYNCED, SYNC_LOST } SYNC;

void Change_Channel_Task (void);
void Synchronization_Task (void);
void RNG_Generate_Task (void);
void RNG_Harvest_Task (void);
void SPI_Send_Task (void);
void SPI_Receive_Task (void);

#endif /* TASKS_H_ */
