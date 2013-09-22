/*------------------------------------------------------------------*-
  Tasks.C (v1.00)
  ------------------------------------------------------------------

  Task list

-*------------------------------------------------------------------*/

#include "tasks.h"
#include "./spi/spi.h"
#include "./rng/rng.h"

MODE Mode_Flag_G = RECEIVE;
ROLE Role_Flag_G = MASTER;
//ROLE Role_Flag_G = SLAVE;

static tByte SPI_Buffer[AUDIO_FRAME_SIZE] = { 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
											  0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
											  0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
											  0x0A, 0x0A, 0x0A, 0x0A, 0x0A, };

void Change_Channel_Task (void)
{
	tByte channel;

	channel = RNG_Get_Rand();

	// change to random frequency

}

void Synchronization_Task (void)
{

}

void RNG_Generate_Task (void)
{
	RNG_Generate();
}

void RNG_Harvest_Task (void)
{
	RNG_Harvest();
}

void SPI_Send_Task (void)
{
	//LED_ON;
	SPI_Send(SPI_Buffer, AUDIO_FRAME_SIZE);
	//LED_OFF;
}

void SPI_Receive_Task (void)
{
	SPI_Receive(SPI_Buffer, AUDIO_FRAME_SIZE);
}




