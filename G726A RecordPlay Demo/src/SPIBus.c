/*SPI1CONbits
	.PPRE = 0;	//Primary Prescale not set for SLAVE Mode
	.SPRE = 0;	//Secondary Prescale not set for SLAVE Mode
	.MSTEN = 0;	//MASTER Mode Enable bit not set
	.CKP = 0;
	.SSEN = 0;	//No Slave Select Enable bit used
	.CKE = 0;	//Output data changes on transition from active to idle clock state
	.SMP = 0;	//Sample Phase bit not set for SLAVE Mode
	.MODE16 = 0;	//Byte-wide (8-bit) communication used
	.DISSDO = 0;	//SDO pin enabled
	.DISSCK = 0;	//SPI clock enabled
*/

/*SPI1STATbits
	.SPIRBF		//Receive Buffer Full Status.  Auto cleared when read of SPI1BUF performed.
	.SPITBF		//Transmit Buffer Full Status. Auto cleared when data transferred from TXB to SR.
	.SPIROV		//Receive Overflow Flag. Clear to confirm no overflow has occurred.
	.SPISIDL	//Stop in Idle Mode
	.SPIEN = 1	//Enable SPI and configure SCK, SDO, SDI, SS port pins
*/	

void SPISlaveInit(){
	SPI1BUF = 0;	//Clear SPI Buffer reg
	IFS0bits.SPI1IF = 0;	// Clear the Interrupt Flag
	IEC0bits.SPI1IE = 0;	// Disable the Interrupt//
	
	/*SPI1CON Register Settings*/
	SPI1CONbits.PPRE = 0;	//Primary Prescale not set for SLAVE Mode
	SPI1CONbits.SPRE = 0;	//Secondary Prescale not set for SLAVE Mode
	SPI1CONbits.MSTEN = 0;	//MASTER Mode Enable bit not set
	SPI1CONbits.CKP = 0;
	SPI1CONbits.SSEN = 0;	//No Slave Select Enable bit used
	SPI1CONbits.CKE = 0;	//Output data changes on transition from active to idle clock state
	SPI1CONbits.SMP = 0;	//Sample Phase bit not set for SLAVE Mode
	SPI1CONbits.MODE16 = 0;	//Byte-wide (8-bit) communication used
	SPI1CONbits.DISSDO = 0;	//SDO pin enabled
	SPI1CONbits.DISSCK = 0;	//SPI clock enabled
	
	SPI1STATbits.SPIROV		//Receive Overflow Flag. Clear to confirm no overflow has occurred.
	SPI1STATbits.SPIEN = 1	//Enable SPI and configure SCK, SDO, SDI, SS port pins
	
	/*SPI Interrupt Controller Settings*/
	IFS0bits.SPI1IF = 0;	// Clear the Interrupt Flag
	IEC0bits.SPI1IE = 0;	// Keep the Interrupt Disabled//
}