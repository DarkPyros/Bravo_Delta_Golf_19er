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
#include "./AES/aes.h"

MODE Mode_Flag_G = RECEIVE;
SYNC Sync_Flag_G = SYNC_LOST;

ROLE Role_Flag_G = MASTER;
//ROLE Role_Flag_G = SLAVE;

//static tByte Next_Channel = 0;
tByte Next_Channel = 0;

static tByte SPI_Buffer[AUDIO_FRAME_SIZE] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5,
											  0xA5, 0xA5, 0xA5, 0xA5, 0xA5,
											  0xA5, 0xA5, 0xA5, 0xA5, 0xA5,
											  0xA5, 0xA5, 0xA5, 0xA5, 0xA5 };

tByte Sync_Info[AES_SIZE] = { 0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00 };

tByte AES_Encrypt_Buffer[AES_SIZE] = { 0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00,
									   0x00, 0x00, 0x00, 0x00 };

tByte Data_Packet_Buffer[AES_SIZE * 2] = { 0x00, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00,
										   0x00, 0x00, 0x00, 0x00 };

void Schedule_Tasks (void)
{
	//hSCH_Add_Task(Change_Channel_Task, 0, 80, PRE_EMP);
	//hSCH_Add_Task(Get_Next_Channel_Task, RNG_GET_RAND_TASK_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
	hSCH_Add_Task(Encrypt_Data_Task, ENCRYPT_DATA_TASK_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
	//hSCH_Add_Task(Start_Synchronization_Task, 2, 80, CO_OP);

	if (Role_Flag_G == SLAVE)
	{
	   	//hSCH_Add_Task(Slave_Synchronization_Task, 30, 800, CO_OP);
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

void Get_Next_Channel_Task (void)
{
	//RED_LED_ON;
	Next_Channel = RNG_Get_Rand();
	//RED_LED_OFF;
}

void Encrypt_Data_Task (void)
{
	tByte i;

	RED_LED_ON;

	for (i = 0; i < AES_SIZE; i++)
	{
		AES_Encrypt_Buffer[i] = SPI_Buffer[i];
	}

	AES_Encrypt_Data(AES_ADDR, AES_Encrypt_Buffer, Data_Packet_Buffer);

	for (i = 0; i < (sizeof(SPI_Buffer) - AES_SIZE); i++)
	{
		AES_Encrypt_Buffer[i] = SPI_Buffer[i];
	}

	for (i = (sizeof(SPI_Buffer) - AES_SIZE); i < AES_SIZE; i++)
	{
		AES_Encrypt_Buffer[i] = 0;
	}

	AES_Encrypt_Data(AES_ADDR, AES_Encrypt_Buffer, &(Data_Packet_Buffer[AES_SIZE]));

	RED_LED_OFF;
}

void SPI_Send_Task (void)
{
	//LED_ON;
	SPI_Send(SPI_Buffer, AUDIO_FRAME_SIZE);
	//LED_OFF;
}

void SPI_Receive_Task (void)
{
	//LED_ON;
	SPI_Receive(SPI_Buffer, AUDIO_FRAME_SIZE);
	//LED_OFF;
}




