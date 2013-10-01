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

void UART_Init(void);
void UART_TX(tByte const);
tByte UART_TX_Busy (void);
void Convert_To_ASCII_Binary(tByte const * const, tByte * const, tByte const);
void Convert_To_ASCII_Hex(tByte const * const, tByte * const, tByte const);
void Byte_Reverse(tByte * const, tByte const);

#endif /* UART_H_ */
