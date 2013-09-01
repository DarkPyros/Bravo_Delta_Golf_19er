#include "msp430.h"
#include "RF1A.h"
#include "hal_pmm.h"
#include "debug.h"

/*******************
 * Function Definition
 */
void Transmit(unsigned char *buffer, unsigned char length);
void ReceiveOn(void);
void ReceiveOff(void);

void InitButtonLeds(void);
void InitRadio(void);
void Clock_Init(void);
