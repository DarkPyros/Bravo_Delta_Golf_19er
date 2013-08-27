/*
 * debug.h
 *
 *  Created on: Aug 22, 2013
 *      Author: Chris Dossman
 */

#ifndef DEBUG_H_
#define DEBUG_H_

//#define TEXT_BINARY
#define TEXT_HEX
//#define DEBUG

extern unsigned char BinaryText[128];
extern unsigned char HexText[32];
extern unsigned char Timer_Hex[4];

void UART_Init(void);
void UART_TX(unsigned char *, unsigned char);
void Convert_To_ASCII_Binary(unsigned char *, unsigned char *, unsigned char);
void Convert_To_ASCII_Hex(unsigned char *, unsigned char *, unsigned char);
void Byte_Reverse(unsigned char *, unsigned char);

#endif /* DEBUG_H_ */
