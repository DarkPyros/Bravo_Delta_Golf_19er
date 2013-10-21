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
#include "../spi/spi.h"
#include "../rng/rng.h"
#include "../UART/uart.h"

/*------------------------------------------------------------------*-

  Init()

  Initialize the EM430F6147RF900 evaluation board.

-*------------------------------------------------------------------*/
void Init (void)
{
	// Stop watchdog timer
	WDTCTL = WDTPW + WDTHOLD;

	// Setup the flags for signaling the dsPIC
	Flags_Init();

	// Setup LED output
	LED_Init();

	// Increase PMMCOREV level to 3 for proper radio operation
	//SetVCore(3);

	// Setup the clock
	Clock_Init();

	// Use Timer_A0 to provide the timer
	// tick for development purposes
	#ifdef TIMER_TICK_TA0
	   	Timer_A0_Init();
	#endif

	// Setup the RTC
	RTC_Init();

	// Setup the CC430 radio
	Radio_Init();

	// Setup the SPI module
	SPI_Init();

	// Setup the AES module
	AES_Init();

	// Setup random number generator
	RNG_Init();

	// Setup the UART module
	UART_Init();
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
		PMAPCTL = PMAPRECFG;					  // Enable further port remaps
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
	UCSCTL1 = DCORSEL_7;                      // Select DCO range for 19.5MHz operation
	UCSCTL2 = FLLD_0 + 2;                     // Set DCO Multiplier for 19.5MHz
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
	RTCCTL01 &= ~(RTCTEVIFG);               // Clear event interrupt
	RTCCTL01 |= RTCHOLD;                    // RTC hold
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

  Sandwich_Timer_Init()

  Setup a Timer_TA1 capture compare register as a sandwich timer
  in order to give certain while() loops a maximum run time.

-*------------------------------------------------------------------*/
void Sandwich_Timer_Init (void)
{
	tWord current_time;

	// Get the current value of timer TA1
	do
	{
		current_time = TA1R;
	} while (current_time != TA1R);

	// Set Timer_TA1 capture compare register 1 to the current
	// timer value plus the (TIMER_TICK_RATE - 50), which is used
	// to give certain while() loops a maximum run time.
	TA1CCR0 = current_time + (TIMER_TICK_RATE - 50);

	// ~(CAP) = set mode to output compare
	// ~(CCIE) = disable the timer interrupt
	// ~(CCIFG) = clear any pending interrupt flags
	// When the timer value, TA1R, equals TA1CCR0,
	// the CCIFG bit will be set in TA1CCTL0
	TA1CCTL0 = ~(CAP + CCIE + CCIFG);
}

/*------------------------------------------------------------------*-

  LED_Init()

  Initialize the LED in order to use it for debugging

-*------------------------------------------------------------------*/
void LED_Init (void)
{
	LED_PORT_DIR |= LED;
	LED_PORT_SEL &= ~LED;
	LED_OFF;

	RED_LED_PORT_DIR |= RED_LED;
	RED_LED_PORT_SEL &= ~RED_LED;
	RED_LED_OFF;
}

/*------------------------------------------------------------------*-

  Flags_Init()

  Initialize the flag pins used for signaling the dsPIC what we
  are going to do next.

-*------------------------------------------------------------------*/
void Flags_Init (void)
{
	P3DIR |= (BIT7);
	P3SEL &= ~(BIT7);
	P3OUT &= ~(BIT7);

	FLAG_PORT_DIR |= RECORD_FLAG + PLAYBACK_FLAG + START_FLAG;
	FLAG_PORT_SEL &= ~(RECORD_FLAG + PLAYBACK_FLAG + START_FLAG);
}

/*------------------------------------------------------------------*-

  Set_Start_Flag()

  Pull the start flag high to signal to the dsPIC that we are
  ready to receive timer ticks. This should be called after
  interrupts have been enabled.

-*------------------------------------------------------------------*/
void Set_Start_Flag (void)
{
	FLAG_PORT |= START_FLAG;
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
		// Enable further port remaps
		PMAPCTL = PMAPRECFG;
		// Map Timer_TA0 output to P3.5
		P3MAP5 = PM_TA0CCR0A;
		// Lock port mapping registers
		PMAPPWD = 0;

		// Set P3.6 as an output
		// and enable its peripheral functions
		// P3.6 = PM_TA0CCR0A
		P3DIR |= BIT5;
		P3SEL |= BIT5;

		// Set TAIDEX to 0 for a clock divider of /1
		TA0EX0 = TAIDEX_0;

		// SMCLK / 1 = 13 MHz / 1 = 13 MHz
		// 1625 / 13 MHz = 125 us
		// 1624 seems to give a more consistent result of 125.1 us, possibly
		// due to variations in SMCLK
		TA0CCR0 = 1624;

		// ~(CAP) = set mode to output compare
		// ~(CCIE) = disable the timer interrupt
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
