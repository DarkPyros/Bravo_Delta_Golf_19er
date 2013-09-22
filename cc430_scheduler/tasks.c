/*------------------------------------------------------------------*-
  Tasks.C (v1.00)
  ------------------------------------------------------------------

  Task list

-*------------------------------------------------------------------*/

#include "tasks.h"
#include "./hSCH/hSCH.h"
#include "./spi/spi.h"
#include "./rng/rng.h"
#include "./radio/radio.h"

MODE Mode_Flag_G = RECEIVE;
ROLE Role_Flag_G = MASTER;
SYNC Sync_Flag_G = SYNC_LOST;
//ROLE Role_Flag_G = SLAVE;

static tByte SPI_Buffer[AUDIO_FRAME_SIZE] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5,
											  0xA5, 0xA5, 0xA5, 0xA5, 0xA5,
											  0xA5, 0xA5, 0xA5, 0xA5, 0xA5,
											  0xA5, 0xA5, 0xA5, 0xA5, 0xA5 };

tByte Sync_Info[AES_SIZE] = { 0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00 };

void Schedule_Tasks (void)
{
	hSCH_Add_Task(Change_Channel_Task, 0, 80, PRE_EMP);
	hSCH_Add_Task(RNG_Generate_Task, 1, 1280, CO_OP);
	hSCH_Add_Task(Start_Synchronization_Task, 2, 80, CO_OP);
	hSCH_Add_Task(RNG_Harvest_Task, 4, 1280, CO_OP);

	if (Role_Flag_G == SLAVE)
	{
	   	hSCH_Add_Task(Slave_Synchronization_Task, 30, 800, CO_OP);
	}
}

void Change_Channel_Task (void)
{
	//tByte channel;

	//channel = RNG_Get_Rand();

	// change to random frequency

}

void Start_Synchronization_Task (void)
{
	if (Role_Flag_G == MASTER)
	{
		tByte * Sync_Info = RNG_Get_Nonce();
		//Radio_Transmit(Sync_Info, AES_SIZE);
	}
	else // Sync_Flag_G == SYNCED or SYNC_LOST
	{
		//Radio_Receive();
	}
}

void Slave_Synchronization_Task (void)
{
	static tByte Sync_Counter = 0;

	if (Sync_Flag_G == SYNCED)
	{
		Radio_Read_RX_FIFO((tByte *)&Sync_Info, AES_SIZE);

	}
	else
	{
		Sync_Counter++;

	}
}

void RNG_Generate_Task (void)
{
	//LED_ON;
	RNG_Generate();
	//LED_OFF;
}

void RNG_Harvest_Task (void)
{
	//LED_ON;
	RNG_Harvest();
	//LED_OFF;
}

void SPI_Send_Task (void)
{
	LED_ON;
	SPI_Send(SPI_Buffer, AUDIO_FRAME_SIZE);
	LED_OFF;
}

void SPI_Receive_Task (void)
{
	//LED_ON;
	SPI_Receive(SPI_Buffer, AUDIO_FRAME_SIZE);
	//LED_OFF;
}




