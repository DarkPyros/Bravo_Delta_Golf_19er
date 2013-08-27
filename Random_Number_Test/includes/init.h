/*
 * init.h
 *
 *  Created on: Aug 22, 2013
 *      Author: Chris Dossman
 */

#ifndef INIT_H_
#define INIT_H_

#include <msp430.h>
#include <stdlib.h>
#include "debug.h"
#include "aes.h"

void Init (void);
void Board_Init (void);
void Clock_Init (void);
void RTC_Init (void);
void Timer_Init (void);
void Timer_Reset (void);

#endif /* INIT_H_ */
