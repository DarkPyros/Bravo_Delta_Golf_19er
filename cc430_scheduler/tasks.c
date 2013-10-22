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
#include "./UART/uart.h"
#include "../hSCH430/hSCH430.h"

MODE Mode_Flag_G = RECEIVE;
SYNC Sync_Flag_G = SYNC_LOST;

//ROLE Role_Flag_G = MASTER;
ROLE Role_Flag_G = SLAVE;

//static tByte Next_Channel = 0;
tByte Next_Channel = 0;

static tByte SPI_Buffer[AUDIO_FRAME_SIZE] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e,
											  0x40, 0x9f, 0x96, 0xe9, 0x3d,
											  0x7e, 0x11, 0x73, 0x93, 0x17,
											  0x2a, 0xA5, 0xA5, 0xA5, 0xA5 };

tByte Sync_Info[AES_SIZE] = { 0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00,
							  0x00, 0x00, 0x00, 0x00 };

tByte AES_Crypto_Buffer[AES_SIZE] = { 0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x00 };

tByte Radio_Data_Packet_Buffer[AES_SIZE * 2] = { 0x00, 0x00, 0x00, 0x00,
										         0x00, 0x00, 0x00, 0x00,
										         0x00, 0x00, 0x00, 0x00,
										         0x00, 0x00, 0x00, 0x00,
										         0x00, 0x00, 0x00, 0x00,
										         0x00, 0x00, 0x00, 0x00,
										         0x00, 0x00, 0x00, 0x00,
										         0x00, 0x00, 0x00, 0x00 };

extern tByte HexText[AES_SIZE * 4];

void Task_Zero (void)
{
	tByte status;

	//LED_ON;

	do {

		status = Strobe(RF_SNOP);

	} while (((status & RF_STATE) != RF_STATE_IDLE) && ((status & RF_STATE) != RF_STATE_RX) && ((status & RF_STATE) != RF_STATE_TX));

	//Strobe(RF_SCAL);

	if (Role_Flag_G == MASTER)
		Strobe(RF_SIDLE);
	else if (Sync_Flag_G == SYNC_LOST)
	{
		Radio_Receive_On();
		Radio_Enable_RX_Interrupt();
	}
	else
	{
		Strobe(RF_SRX);
	}

	//do {

	//	status = Strobe(RF_SNOP);

	//} while (((status & RF_STATE) != RF_STATE_IDLE) && ((status & RF_STATE) != RF_STATE_RX) && ((status & RF_STATE) != RF_STATE_TX));

	//LED_OFF;
}

void Schedule_Tasks (void)
{
	if (Role_Flag_G == MASTER)
	{
		//hSCH_Add_Task(Change_Channel_Task, RADIO_CHANGE_CHANNEL_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
		//hSCH_Add_Task(Get_Next_Channel_Task, RNG_GET_RAND_TASK_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
		hSCH_Add_Task(Task_Zero, TASK_ZERO_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
		hSCH_Add_Task(Start_Synchronization_Task, START_SYNC_TASK_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
		//hSCH_Add_Task(Encrypt_Data_Task, ENCRYPT_DATA_TASK_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
		//hSCH_Add_Task(UART_Send_Task, UART_SEND_DATA_TASK_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
		hSCH_Add_Task(Poll_Transmit_Button_Task, POLL_TRANSMIT_TASK_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
		hSCH_Add_Task(Task_Manager, TASK_MANAGER_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
	}
	else
	{
		hSCH_Add_Task(Task_Zero, TASK_ZERO_DELAY, (TICKS_PER_FRAME - 1), CO_OP);
	}
}

void Sync_Schedule_Tasks (void)
{
	hSCH_Add_Task(Raise_Start_Flag_Task, RAISE_START_FLAG_TASK_DELAY, 0, CO_OP);

	hSCH_Add_Task(Task_Zero, (RAISE_START_FLAG_TASK_DELAY + TASK_ZERO_DELAY), (TICKS_PER_FRAME - 1), CO_OP);
	hSCH_Add_Task(Poll_Transmit_Button_Task, (RAISE_START_FLAG_TASK_DELAY + POLL_TRANSMIT_TASK_DELAY), (TICKS_PER_FRAME - 1), CO_OP);
	hSCH_Add_Task(Task_Manager, (RAISE_START_FLAG_TASK_DELAY + TASK_MANAGER_DELAY), (TICKS_PER_FRAME - 1), CO_OP);

	hSCH_Add_Task(Slave_Synchronization_Task, (RAISE_START_FLAG_TASK_DELAY + SLAVE_SYNC_TASK_DELAY), (TICKS_PER_FRAME - 1), CO_OP);
}

void Raise_Start_Flag_Task (void)
{
	FLAG_PORT |= START_FLAG;
	hSCH_Reset_Tick_Counter();
}

void Change_Channel_Task (void)
{
	tByte channel = 0;
	tByte status;

	// channel = RNG_Get_Rand();

	// change to random frequency

	LED_ON;

	do {

		status = Strobe(RF_SNOP);

	} while (((status & RF_STATE) != RF_STATE_IDLE) && ((status & RF_STATE) != RF_STATE_RX) && ((status & RF_STATE) != RF_STATE_TX));

	WriteSingleReg(CHANNR, channel++);

	//Strobe(RF_SIDLE);

	Strobe(RF_SRX);

	if(channel > 127)
		channel = 0;

	do {

		status = Strobe(RF_SNOP);

	} while (((status & RF_STATE) != RF_STATE_IDLE) && ((status & RF_STATE) != RF_STATE_RX) && ((status & RF_STATE) != RF_STATE_TX));

	LED_OFF;

	//Strobe(RF_SRX);

}

void Start_Synchronization_Task (void)
{
	if (Role_Flag_G == MASTER)
	{
		tByte * Nonce_Info = RNG_Get_Nonce();
		Radio_Transmit(Nonce_Info, AES_SIZE);
	}
	else // Sync_Flag_G == SYNCED or SYNC_LOST
	{
		Radio_Receive_On();
	}
}

void Slave_Synchronization_Task (void)
{
	tByte Sync_Result;
	static tByte Sync_Counter = 0;

	Sync_Result = Radio_Read_RX_FIFO(Sync_Info, sizeof(Sync_Info));

	if (Sync_Result != AES_SIZE)
		Sync_Counter++;
	else
	{
		Sync_Counter = 0;
		Sync_Flag_G = SYNCED;

		Overwrite_Nonce(Sync_Info);


	}

	if (Sync_Counter >= SYNC_LOST_THRESHOLD)
	{
		Sync_Counter = SYNC_LOST_THRESHOLD;
		Sync_Flag_G = SYNC_LOST;
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
		AES_Crypto_Buffer[i] = SPI_Buffer[i];
	}

	AES_Encrypt_Data(AES_ADDR, AES_Crypto_Buffer, Radio_Data_Packet_Buffer);

	for (i = 0; i < (sizeof(SPI_Buffer) - AES_SIZE); i++)
	{
		AES_Crypto_Buffer[i] = SPI_Buffer[i];
	}

	for (i = (sizeof(SPI_Buffer) - AES_SIZE); i < AES_SIZE; i++)
	{
		AES_Crypto_Buffer[i] = 0xAA;
	}

	AES_Encrypt_Data(AES_ADDR, AES_Crypto_Buffer, (tByte *) &(Radio_Data_Packet_Buffer[AES_SIZE]));

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

void UART_Send_Task (void)
{
	static tByte UART_Index = 0;
	static tByte UART_Convert = FALSE;

	RED_LED_ON;

	if ((UART_Index == 0) && (UART_Convert == FALSE))
	{
		UART_Convert = TRUE;
		Convert_To_ASCII_Hex(Radio_Data_Packet_Buffer, HexText, sizeof(Radio_Data_Packet_Buffer));
		//Byte_Reverse(HexText, sizeof(HexText));
	}

	if (UART_TX_Busy() == FALSE)
	{
		UART_TX(HexText[UART_Index++]);

		if (UART_Index >= sizeof(HexText))
		{
			UART_Index = 0;
			UART_Convert = FALSE;
		}
	}

	RED_LED_OFF;
}

void Task_Manager (void)
{
	Strobe(RF_SIDLE);

}

void Poll_Transmit_Button_Task (void)
{
	static tByte Duration;

	if (TX_BUTTON_PRESSED)
	{
		Duration += 1;
		if (Duration > SW_THRES)
		{
			Duration = SW_THRES;
			FLAG_PORT &= ~(PLAYBACK_FLAG);
			FLAG_PORT |= (RECORD_FLAG);
			Mode_Flag_G = TRANSMIT;

			hSCH_Add_Task(SPI_Receive_Task, (SPI_RECEIVE_TASK_DELAY - 1), 0, CO_OP);
			hSCH_Add_Task(Encrypt_Data_Task, (ENCRYPT_DATA_TASK_DELAY - 1), 0, CO_OP);
			hSCH_Add_Task(Transmit_Data_Task, (TRANSMIT_DATA_TASK_DELAY - 1), 0, CO_OP);
			return;
		}
		// Switch pressed, but not yet for long enough
		return;
	}
	// Switch not pressed - reset the count
	else
	{
		Duration = 0;
		FLAG_PORT &= ~(RECORD_FLAG + PLAYBACK_FLAG);
		Mode_Flag_G = RECEIVE;

		hSCH_Add_Task(Receiving_Mode_Task, (RECEIVING_MODE_TASK_DELAY - 1), 0, CO_OP);
		hSCH_Add_Task(Check_RX_Data_Task, (CHECK_RX_DATA_TASK_DELAY - 1), 0, CO_OP);
	}
}

void Transmit_Data_Task (void)
{
	Radio_Transmit (Radio_Data_Packet_Buffer, sizeof(Radio_Data_Packet_Buffer));
}

void Receiving_Mode_Task (void)
{
	Radio_Receive_On();
}

void Check_RX_Data_Task (void)
{
	tByte RX_FIFO_SIZE = Radio_Read_RX_FIFO(Radio_Data_Packet_Buffer, sizeof(Radio_Data_Packet_Buffer));
	if((RX_FIFO_SIZE > 0) && (RX_FIFO_SIZE != 0xFF))
	{
		hSCH_Add_Task(Decrypt_Data_Task, (DECRYPT_DATA_TASK_DELAY -1), 0, CO_OP);
		hSCH_Add_Task(SPI_Send_Task, (SPI_SEND_TASK_DELAY - 1), 0, CO_OP);
		FLAG_PORT &= ~(RECORD_FLAG);
		FLAG_PORT |= (PLAYBACK_FLAG);
	}
}

void Decrypt_Data_Task (void)
{
	tByte i;

	RED_LED_ON;

	for (i = 0; i < AES_SIZE; i++)
	{
		AES_Crypto_Buffer[i] = Radio_Data_Packet_Buffer[i];
	}

	AES_decryptDataUsingEncryptionKey(AES_ADDR, AES_Crypto_Buffer, SPI_Buffer);

	for (i = 0; i < (sizeof(Radio_Data_Packet_Buffer) - AES_SIZE); i++)
	{
		AES_Crypto_Buffer[i] = Radio_Data_Packet_Buffer[i];
	}

	for (i = (sizeof(Radio_Data_Packet_Buffer) - AES_SIZE); i < AES_SIZE; i++)
	{
		AES_Crypto_Buffer[i] = 0xAA;
	}

	AES_decryptDataUsingEncryptionKey(AES_ADDR, AES_Crypto_Buffer, (tByte *) &(SPI_Buffer[AES_SIZE]));

	RED_LED_OFF;
}


