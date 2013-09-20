/*------------------------------------------------------------------*-

  Init.C (v1.00)

  ------------------------------------------------------------------

  *** Hardware Initializations for CC430

        - Clock = 19.5 MHz from RF XT2
        - Timer_A0 toggles an output pin every 125 us
        - Radio operates in 902 - 928 MHz
        - Real-Time Clock

-*------------------------------------------------------------------*/
#include "init.h"
#include "../radio/radio.h"

/*------------------------------------------------------------------*-

  Init()

  Initialize the EM430F6147RF900 evaluation board.

-*------------------------------------------------------------------*/
void Init (void)
{
	// Stop watchdog timer
	WDTCTL = WDTPW + WDTHOLD;

	// Setup the clock
	Clock_Init();

	// Setup the RTC
	RTC_Init();

	// Setup the nonce timer
	Timer_Init();

	//Setup the CC430 radio
	Radio_Init();

	// Use Timer_A0 to provide the timer
	// tick for development purposes
	#ifdef TIMER_TICK_TA0
	   	Timer_A0_Init();
	#endif
}


/*------------------------------------------------------------------*-

  Clock_Init()

  Initialize system clock to run at 19.5 MHz using the 26 MHz RF
  crystal on the EM430F6147RF900 evaluation board.

-*------------------------------------------------------------------*/
void Clock_Init (void)
{
	// Setup Clock
	P5SEL |= BIT0 + BIT1;                     // Port select XT1
	UCSCTL6 |= XT1OFF + XT2OFF;               // XT1 and XT2 off

	// Loop until XT1, XT2 & DCO stabilizes - In this case loop until XT1, XT2 and DCO settle
	do
	{
		UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT1HFOFFG + XT2OFFG);
		                                      // Clear XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;		              // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	UCSCTL6 &= ~(XT2OFF);                     // Turn on XT2

	P5SEL &= ~(BIT0 + BIT1);                  // Port select ~XT1

	#ifdef DEBUG_CLOCK_OUTPUT
		PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
		P2MAP0 = PM_ACLK;                         // Map ACLK output to P2.0
		P2MAP2 = PM_MCLK;                         // Map MCLK output to P2.2
		P2MAP4 = PM_SMCLK;                        // Map SMCLK output to P2.4
		PMAPPWD = 0;                              // Lock port mapping registers

		P2DIR |= BIT0 + BIT2 + BIT4;              // ACLK, MCLK, SMCLK set out to pins
		P2SEL |= BIT0 + BIT2 + BIT4;              // P2.0,2,4 for debugging purposes.
	#endif

	UCSCTL3 = SELREF_5 + FLLREFDIV_2;         // Set DCO FLL reference = XT2
	                                          // Set FLLREFDIV = /4
	UCSCTL4 = SELA_4 + SELS_5 + SELM_4;       // Set ACLK = DCOCLKDIV
	                                          // Set SMCLK = XT2CLK
	                                          // Set MCLK = DCOCLKDIV

	UCSCTL5 = DIVS_1;                         // Set SMCLK = XT2CLK / 2

	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_7;                      // Select DCO range for 20MHz operation
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
		SFRIFG1 &= ~OFIFG;                    // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}

/*------------------------------------------------------------------*-

  RTC_Init()

  Initialize the Real-Time Clock

-*------------------------------------------------------------------*/
void RTC_Init(void)
{
	// Configure RTC_D
	RTCCTL01 &= ~(RTCTEVIFG);               //Clear event interrupt
	RTCCTL01 |= /*RTCTEVIE +*/ RTCHOLD;         //RTC hold, enable RTC
	                                        // event interrupt
	RTCYEAR = 2013;                         // Year = 2013
	RTCMON = 07;                            // Month = 7 = July
	RTCDAY = 21;                            // Day = 21 = 21st
	RTCDOW = 00;                            // Day of week = 0 = Sunday
	RTCHOUR = 22;                           // Hour = 22
	RTCMIN = 59;                            // Minute = 59
	RTCSEC = 55;                            // Seconds = 55
	RTCCTL01 &= ~(RTCHOLD);                 // Start RTC calendar mode
}

/*------------------------------------------------------------------*-

  Timer_Init()

  Initialize Timer_TA1 for the microsecond nonce counter

-*------------------------------------------------------------------*/
void Timer_Init (void)
{
	// Reset and activate timer TA1
	Timer_Reset();
}

/*------------------------------------------------------------------*-

  Timer_Reset()

  Reset Timer_TA1 for the microsecond nonce counter

-*------------------------------------------------------------------*/
void Timer_Reset (void)
{
	// Set TAIDEX to 4 for a clock divider of /5
	TA1EX0 = 0x0004;

	// TASSEL_2 = set timer clock source to SMCLK
	// ID_2 = set clock divider to /4, along with TAIDEX for
	//        a total clock divider of /20
	// MC_2 = continuous up counting
	// TACLR = resets and activates the timer counter
	TA1CTL = TASSEL_2 + ID_2 + MC_2 + TACLR;	//Reset and activate Counter
}

/*------------------------------------------------------------------*-

  Timer_A0_Init()

  Setup Timer_A0 to toggle the output of P3.6 in order to provide
  a 125 us timer tick source during development. A jumper should
  be set from P3.6 to TIMER_TICK_INPUT_PIN in order to use this as
  the timer tick source.

-*------------------------------------------------------------------*/
#ifdef TIMER_TICK_TA0
	void Timer_A0_Init (void)
	{
		// Get write-access to port mapping regs
		PMAPPWD = 0x02D52;
		// Map Timer_TA0 output to P3.6
		P3MAP6 = PM_TA0CCR0A;
		// Lock port mapping registers
		PMAPPWD = 0;

		// Set P3.6 as an output
		// and enable its peripheral functions
		// P3.6 = PM_TA0CCR0A
		P3DIR |= BIT6;
		P3SEL |= BIT6;

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
#endif

