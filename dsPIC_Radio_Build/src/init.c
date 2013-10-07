#include "../h/includes.h"

/* Initialization of dsPIC Starter Kit*/
void INIT_init(void) {
	
	INIT_clockInit();
	INIT_portInit();

#ifndef USE_SFM_CHIP
	SCH_initExtTrigger();
	SPI_SlaveInit();
#endif

	/* Set operating mode to IDLE */
	modeFlag 	= IDLE;
	currentMode = IDLE;
} /* End of INIT_init() */

/* Initialization of dsPIC ports*/
void INIT_portInit(void) {

	/* Initialize PortB pins as inputs for mode selection */
	PLAYBACK_TRIS = 1;
	RECORD_TRIS   = 1;
	
	/* Initialize the board LED and switch ports	
	 * and turn all LEDS off. */
	 
	YELLOW_LED_TRIS	= 0;
	RED_LED_TRIS = 0;		
 	GREEN_LED_TRIS = 0;	

 	YELLOW_LED = SASK_LED_OFF;	
	RED_LED	= SASK_LED_OFF;		
 	GREEN_LED = SASK_LED_OFF;		

 	SWITCH_S1_TRIS = 1;	
 	SWITCH_S2_TRIS	= 1;
 	
	VOLUME_UPDN_TRIS = 0;	
	VOLUME_CLK_TRIS	= 0;
	
	VOLUME_UPDN = 0;
	VOLUME_CLK = 0;	
	
	debounceS1 		= 0;
	debounceS2 		= 0;
} /* End of INIT_boardInit() */

/* Initialization of dsPIC oscillator clock*/
void INIT_clockInit(void) {

	/* Configure Oscillator to operate the device at 80MHz / 40 MIPS.
	 * Fosc = Fin*M/(N1*N2); Fcy = Fosc/2
	 * Fosc = 12*40/(3*2) = 80Mhz for 12MHz input clock */ 
	PLLFBD=38;				/* M  = PLLFBD  + 2 = 40	*/
	CLKDIVbits.PLLPRE=1;	/* N1 = PLLPRE  + 2 = 3		*/
	CLKDIVbits.PLLPOST=0;	/* N2 = PLLPOST + 2 = 2		*/
	OSCTUN=0;			
	
	__builtin_write_OSCCONH(0x01);		/* Initiate Clock Switch to FRC with PLL */
	__builtin_write_OSCCONL(0x01);
	while (OSCCONbits.COSC != 0b01);	/* Wait for Clock switch to occur */
	while(!OSCCONbits.LOCK);
} /* End of INIT_clockInit() */
