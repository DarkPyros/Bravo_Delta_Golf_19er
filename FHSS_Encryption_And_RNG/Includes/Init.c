#include <msp430.h>
#include "Init.h"

void Clock_Init(void)
{
  // Setup Clock
  P5SEL |= BIT0 + BIT1;                     // Port select XT1
  UCSCTL6 |= XCAP_3 + XT1OFF + XT2OFF;      // Internal load cap
                                            // XT1 and XT2 off
  // Loop until XT1 & DCO stabilizes - In this case loop until XT1 and DCO settle
  do
  {
	UCSCTL7 &= ~(XT1LFOFFG + DCOFFG);
                                            // Clear XT1,DCO fault flags
	SFRIFG1 &= ~OFIFG;                  	// Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive
                                            // strength
/*
  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P2MAP0 = PM_ACLK;                         // Map ACLK output to P2.0
  P2MAP2 = PM_MCLK;                         // Map MCLK output to P2.2
  P2MAP4 = PM_SMCLK;                        // Map SMCLK output to P2.4
  PMAPPWD = 0;                              // Lock port mapping registers
*/

  P2DIR |= BIT0 + BIT2 + BIT4;              // ACLK, MCLK, SMCLK set out to pins
  P2SEL |= BIT0 + BIT2 + BIT4;              // P2.0,2,4 for debugging purposes.

  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_7;                      // Select DCO range 20MHz operation
  UCSCTL2 = FLLD_1 + 614;                   // Set DCO Multiplier for 12MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (614 + 1) * 32768 = 20MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 20 MHz / 20,000 Hz = 625000 = MCLK cycles for DCO to settle
  __delay_cycles(625000);

  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

}

void Board_Init(void)
{
  PMMCTL0_H = PMMPW_H;                      // open PMM
  PM5CTL0 &= ~LOCKIO;                       // Clear LOCKBAK and enable ports
  PMMCTL0_H = 0x00;                         // close PMM

  // Port Configuration
  P1OUT = 0x80;P2OUT = 0x00;P3OUT = 0x00;P4OUT = 0x00;P5OUT = 0x00;PJOUT = 0x00;
  P1DIR = 0x6F;P2DIR = 0xFF;P3DIR = 0xFF;P4DIR = 0xFF;P5DIR = 0xFF;PJDIR = 0xFF;
  P1REN = 0x90;
}

void RTC_Init(void)
{
  // Configure RTC_D
  RTCCTL01 &= ~(RTCTEVIFG);                //Clear event interrupt
  RTCCTL01 |= RTCTEVIE + RTCHOLD;         //RTC hold, enable RTC
                                          // event interrupt
  RTCYEAR = 2013;                         // Year = 2011
  RTCMON = 07;                            // Month = 10 = October
  RTCDAY = 21;                            // Day = 7 = 7th
  RTCDOW = 05;                            // Day of week = 5 = Friday
  RTCHOUR = 22;                           // Hour = 11
  RTCMIN = 59;                            // Minute = 59
  RTCSEC = 55;                            // Seconds = 55
  //RTCCTL01|= RTCTEV_0;                      // Set RTCTEV for 1 minute alarm
  RTCCTL01 &= ~(RTCHOLD);                   // Start RTC calendar mode
}

void UART_Init (void)
{
   P1SEL |= BIT5 + BIT6;

   UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
   UCA0CTL0 = 0x00;                          // UART - 8/N/1 with LSB first
   UCA0CTL1 |= UCSSEL_2;                     // SMCLK
   UCA0BR0 = 0x5B;                           // 20MHz 57600 (see User's Guide)
   UCA0BR1 = 1;                              // 20MHz 57600
   UCA0MCTL |= UCBRS_2 + UCBRF_0;            // Modulation UCBRSx=2, UCBRFx=0
   UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}
