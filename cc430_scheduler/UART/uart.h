/*
 * uart.h
 *
 *  Created on: Sep 30, 2013
 *      Author: Chris
 */

#ifndef UART_H_
#define UART_H_

#include "../main.h"
#include <stdlib.h>

#define UART_BAUD_RATE		(115200)
#define UART_BUFFER_SIZE 	(37)

#define UART_TX_FLAG_DOWN (0)
#define UART_TX_FLAG_UP   (1)

void UART_Init (void);
void UART_TX (void);
void UART_TX_Buffer_Write (tByte const *, tByte);
void Convert_To_ASCII_Binary (tByte const * const, tByte * const, tByte const);
void Convert_To_ASCII_Hex (tByte const * const, tByte * const, tByte const);
void Byte_Reverse (tByte * const, tByte const);

#endif /* UART_H_ */
