/*------------------------------------------------------------------*-
  SPI.H (v1.00)
  ------------------------------------------------------------------

  Header file for the CC430 SPI module

-*------------------------------------------------------------------*/

#ifndef SPI_H_
#define SPI_H_

#include "../main.h"
#include "../init/init.h"

void SPI_Init (void);
void SPI_Send (tByte const * const, tByte);
void SPI_Receive (tByte * const, tByte);

#endif /* SPI_H_ */
