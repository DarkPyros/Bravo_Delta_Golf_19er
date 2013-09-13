#include <msp430.h> 

/*
 * main.c
 */

void Clock_Init(void);
void Timer_A0_Init (void);
void Timer_Tick_Init(void);

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer

    Clock_Init();
    Timer_A0_Init();
    Timer_Tick_Init();

    P3DIR = 0xFF;
    P3OUT |= BIT6; // on

    P1DIR |= BIT0;
    P1OUT |= BIT0;

    __bis_SR_register( GIE );       // Enter LPM4 w/interrupt
    __no_operation();                         // For debugger

    while (1)
    {
    	P3OUT ^= BIT6;

    }
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
P1OUT ^= BIT0;    // P1.0 = toggle LED
P1IFG &= ~BIT1; // P1.1 IFG cleared
// P1IES ^= BUTTON; // toggle the interrupt edge,
// the interrupt vector will be called
// when P1.3 goes from HitoLow as well as
// LowtoHigh
}

void Timer_Tick_Init(void)
{
  // Set up P1.1 as the timer tick interrupt source
  // P1SEL disables the P1.1 peripheral function
  // P1DIR sets P1.1 as an input
  // P1REN enables the pull-up/down resistor
  // P1IES sets a high-to-low transition interrupt edge select
  // P1IFG clears any pending interrupts
  // P1OUT sets the pull resistor as a pull-up
  // P1IE enables the interrupt
  P1SEL &= ~BIT1;
  P1DIR &= ~BIT1;
  P1REN |= BIT1;
  P1OUT |= BIT1;
  P1IE  |= BIT1;
  P1IES |= BIT1;
  P1IFG &= ~BIT1;
}

void Clock_Init(void)
{
  // Setup Clock
  P5SEL |= BIT0 + BIT1;                     // Port select XT1
  UCSCTL6 |= XT1OFF + XT2OFF;               // XT1 and XT2 off

  // Loop until XT1, XT2 & DCO stabilizes - In this case loop until XT1, XT2 and DCO settle
  do
  {
	UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT1HFOFFG + XT2OFFG);
                                            // Clear XT1,DCO fault flags
	SFRIFG1 &= ~OFIFG;                  	// Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL6 &= ~(XT2OFF);                     // Turn on XT2

  P5SEL &= ~(BIT0 + BIT1);                  // Port select ~XT1

  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P2MAP0 = PM_ACLK;                         // Map ACLK output to P2.0
  P2MAP2 = PM_MCLK;                         // Map MCLK output to P2.2
  P2MAP4 = PM_SMCLK;                        // Map SMCLK output to P2.4
  PMAPPWD = 0;                              // Lock port mapping registers


  P2DIR |= BIT0 + BIT2 + BIT4;              // ACLK, MCLK, SMCLK set out to pins
  P2SEL |= BIT0 + BIT2 + BIT4;              // P2.0,2,4 for debugging purposes.

  UCSCTL3 = SELREF_5 + FLLREFDIV_2;         // Set DCO FLL reference = XT2
                                            // Set FLLREFDIV = /4
  UCSCTL4 = SELA_4 + SELS_5 + SELM_4;       // Set ACLK = DCOCLKDIV
                                            // Set SMCLK = XT2CLK
                                            // Set MCLK = DCOCLKDIV

  UCSCTL5 = DIVS_1;                         // Set SMCLK = XT2CLK / 2

  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_7;                      // Select DCO range 20MHz operation
  UCSCTL2 = FLLD_0 + 2;                     // Set DCO Multiplier for 20MHz
                                            // ((N + 1) * FLLRef) / FLLDiv = Fdco
                                            // ((2 + 1) * 6.5 MHz) / 1 = 19.5MHz
                                            // Set FLL Div = fDCOCLK/1
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 4 x 32 x 32 x 19.5 MHz / 26 MHz = 3072 MCLK cycles for DCO to settle
  __delay_cycles(3072);

  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}


void Timer_A0_Init (void)
{
	// Set P3.1 as an output
	// and enable its peripheral functions
	// P3.1 = PM_TA0CCR0A
	P3DIR |= BIT1;
	P3SEL |= BIT1;

	// Set TAIDEX to 0 for a clock divider of /1
	TA0EX0 = TAIDEX_0;

	// SMCLK / 1 = 13 MHz / 1 = 13 MHz
	// 1625 / 13 MHz = 125 us
	// 1624 seems to give a more consistent result of 125.1 us, possibly
	// due to variations in SMCLK
	TA0CCR0 = 1624;

	// ~(CAP) = set mode to output compare
	// OUTMOD_5 = toggle output pin
	// OUTMOD_5 operates correctly as the toggle mode even though the user guide
	// says that toggle mode is OUTMOD_4. OUTMOD_4 seems to operate as set/reset.
	TA0CCTL0 = ~(CAP + CCIE) + OUTMOD_5;

	// TASSEL_2 = set timer clock source to SMCLK
	// ID_0 = set clock divider to /1, along with TAIDEX for
	//        a total clock divider of /2
	// MC_1 = timer counts up to TA0CCR0
	// TACLR = resets and activates the timer counter
	TA0CTL = TASSEL_2 + ID_0 + MC_1 + TACLR;
}

