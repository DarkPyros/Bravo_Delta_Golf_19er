/*------------------------------------------------------------------*-
  SPI.C (v1.00)
  ------------------------------------------------------------------

  CC430 SPI module

-*------------------------------------------------------------------*/

#include "spi.h"

/*------------------------------------------------------------------*-

  SPI_Init()

  Initialize the USCI_B0 SPI module to use 3-wire SPI, 8-bit, MSB
  first, clock polarity high, clock phase high, and SPI Master
  mode. The bit rate clock is sourced from SMCLK with a bit rate
  divider of /3 (with an SMCLK of 13 MHz that gives an SPI speed of
  13 MHz / 3 = ~4.33 Mbit/s).

  20 bytes of audio data should take:

  1/(4.33 Mbit/s) * 8 bits/byte * 20 bytes = ~ 37 us

-*------------------------------------------------------------------*/
void SPI_Init (void)
{
	// Enable P1.2, P1.3, P1.4 as outputs and enable
	// their peripheral functions (SPI)
	// P1.2 is Slave-Out, Master-In
	// P1.3 is Slave-In, Master-Out
	// P1.4 is the Clock signal
	P1DIR |= BIT2 + BIT3 + BIT4;
	P1SEL |= BIT2 + BIT3 + BIT4;

	// UCSSEL_2 - selects SMCLK as the clock source
	// UCB0CTL0 - UCCKPH is clock phase high
	//          - UCCKPL is clock polarity high
	//          - UCMSB is MSB first
	//          - UCMST is master mode
	//          - UCMODE_0 is 3-wire SPI mode
	//          - UCSYNC is synchronous serial mode
	// UCB0BR0  - is the source clock divider low byte
	// UCB0BR1  - is the source clock divider high byte
	UCB0CTL1 |= UCSWRST;
	UCB0CTL1 |= UCSSEL_2;
	UCB0CTL0 |= UCCKPH + UCCKPL + UCMSB + UCMST + UCMODE_0 + UCSYNC;
	UCB0BR0 = 3;
	UCB0BR1 = 0;
	UCB0CTL1 &= ~(UCSWRST);
}

/*------------------------------------------------------------------*-

  SPI_Send()

  Send a frame of compressed audio data to the dsPIC (20 bytes).

-*------------------------------------------------------------------*/
void SPI_Send (tByte const * const txData, tByte count)
{
	tByte i;

	Sandwich_Timer_Init();

	for (i = 0; i < count; i++)
	{
		// USCI_B0 TX buffer ready? and sandwich timer not
		// overflowed?
		// This while() loop has a maximum run time of ~97 us
		while (!(UCB0IFG & UCTXIFG) && !(Sandwich_Timer_Overflow()));

		// Send a byte over SPI
		UCB0TXBUF = txData[i];
	}

	// If the sandwich timer overflows, set the global error
	// variable to ERROR_SCH_SPI_TX_ERROR
	if (Sandwich_Timer_Overflow())
		hSCH_Set_Error_Code(ERROR_SCH_SPI_TX_ERROR);
}

/*------------------------------------------------------------------*-

  SPI_Receive()

  Receive a frame of compressed audio data from the dsPIC (20 bytes).

-*------------------------------------------------------------------*/
void SPI_Receive (tByte * const rxData, tByte count)
{
	tByte i;

	Sandwich_Timer_Init();

	for (i = 0; i < count; i++)
	{
		// Send a dummy byte over SPI
		UCB0TXBUF = 0xAA;

		// USCI_B0 RX buffer ready? and sandwich timer not
		// overflowed?
		// This while() loop has a maximum run time of ~97 us
		while (!(UCB0IFG & UCRXIFG) && !(Sandwich_Timer_Overflow()));

		rxData[i] = UCB0RXBUF;
	}

	// If the sandwich timer overflows, set the global error
	// variable to ERROR_SCH_SPI_RX_ERROR
	if (Sandwich_Timer_Overflow())
		hSCH_Set_Error_Code(ERROR_SCH_SPI_RX_ERROR);
}
