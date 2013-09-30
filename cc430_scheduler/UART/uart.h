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
void UART_TX(unsigned char *, unsigned char);
void Convert_To_ASCII_Binary(unsigned char *, unsigned char *, unsigned char);
void Convert_To_ASCII_Hex(unsigned char *, unsigned char *, unsigned char);
void Byte_Reverse(unsigned char *, unsigned char);

#endif /* UART_H_ */
