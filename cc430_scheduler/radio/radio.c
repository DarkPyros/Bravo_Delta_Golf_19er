/*------------------------------------------------------------------*-
  Radio.C (v1.00)
  ------------------------------------------------------------------

  CC430 radio module interface and radio configuration settings.
  The radio is configured to operate in the 902 - 928 MHz ISM band
  while employing frequency hopping spread spectrum (FHSS) within
  that band. It uses 128 channels, equally spaced between
  902.3 MHz and 927.7 MHz, with a channel spacing of 200 KHz. The
  baud rate is 76.8 kBaud with 2-GFSK modulation.

-*------------------------------------------------------------------*/

#include "radio.h"

// Product = CC430F6147
// Chip version = C   (PG 0.7)
// Crystal accuracy = 10 ppm
// X-tal frequency = 26 MHz
// RF output power = 0 dBm
// RX filter bandwidth = 203.125000 kHz
// Deviation = 41.259766 kHz
// Data Rate = 76.767 kBaud
// Modulation = 2-GFSK
// Manchester enable = (0) Manchester disabled
// RF Frequency = 902.299896 MHz
// Channel spacing = 199.951172 kHz
// Channel number = 0
// Optimization = -
// Sync mode = (3) 30/32 sync word bits detected
// Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
// CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
// Forward Error Correction =
// Length configuration = (1) Variable packet length, Max packet length configured by PKTLEN
// Packet Length = 48 bytes max
// Preamble count = (2)  4 bytes
// Append status = 1
// Address check = (0) No address check
// FIFO autoflush = 0
// Device address = 0
// GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet
// GDO2 signal selection = (41) RF_RDY
const RF_SETTINGS rfSettings = {
    0x08,	// FSCTRL1   Frequency Synthesizer Control
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x22,   // FREQ2     Frequency Control Word, High Byte
    0xB4,   // FREQ1     Frequency Control Word, Middle Byte
    0x2F,   // FREQ0     Frequency Control Word, Low Byte
    0x8B,   // MDMCFG4   Modem Configuration
    0x83,   // MDMCFG3   Modem Configuration
    0x13,   // MDMCFG2   Modem Configuration
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.
    0x00,   // CHANNR    Channel number.
    0x45,   // DEVIATN   Modem Deviation Setting
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end TX configuration.
    0x10,   // MCSM0     Main Radio Control State Machine Configuration
    0x16,   // FOCCFG    Frequency Offset Compensation Configuration
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x43,   // AGCCTRL2  AGC Control
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.
	0x80,   // WOREVT1   High byte Event0 timeout
	0x00,   // WOREVT0   Low byte Event0 timeout
	0xFB,   // WORCTRL   Wake On Radio Control
    0xE9,   // FSCAL3    Frequency Synthesizer Calibration
    0x2A,   // FSCAL2    Frequency Synthesizer Calibration
    0x00,   // FSCAL1    Frequency Synthesizer Calibration
    0x1F,   // FSCAL0    Frequency Synthesizer Calibration
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various Test Settings
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x47,   // FIFOTHR   RXFIFO and TXFIFO thresholds.
    0x29,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0    GDO0 output pin configuration. Refer to SmartRF� Studio User Manual for detailed pseudo register explanation.
    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    PACKET_LEN // PKTLEN    Packet length.
};

void Radio_Init (void)
{
	// Set the High-Power Mode Request Enable bit so LPM3 can be entered
	// with active radio enabled
	//PMMCTL0_H = 0xA5;
	//PMMCTL0_L |= PMMHPMRE_L;
	//PMMCTL0_H = 0x00;

	ResetRadioCore();

	WriteRfSettings(&rfSettings);

	WriteSinglePATable(PATABLE_VAL);

	Strobe(RF_SCAL);


}

void Radio_Read_RX_FIFO(tByte * const RX_Buffer, tByte size)
{
	tByte RX_Buffer_Length;

	// Read the length byte from the FIFO
	RX_Buffer_Length = ReadSingleReg( RXBYTES );

	if (RX_Buffer_Length == size)
	{
		ReadBurstReg(RF_RXFIFORD, RX_Buffer, RX_Buffer_Length);

		// Stop here to see contents of RxBuffer
		__no_operation();

		// Check the CRC results
		if(!(RX_Buffer[CRC_LQI_IDX] & CRC_OK))
		{
			Error_code_G = ERROR_RADIO_RX_CRC_BAD;
		}
	}
}

void Transmit(unsigned char *buffer, unsigned char length)
{
  //RF1AIES |= BIT9;
  //RF1AIFG &= ~BIT9;                         // Clear pending interrupts
  //RF1AIE |= BIT9;                           // Enable TX end-of-packet interrupt

  WriteBurstReg(RF_TXFIFOWR, buffer, length);

  Strobe( RF_STX );                         // Strobe STX
}

void ReceiveOn(void)
{
  //RF1AIES |= BIT9;                          // Falling edge of RFIFG9
  //RF1AIFG &= ~BIT9;                         // Clear a pending interrupt
  //RF1AIE  |= BIT9;                          // Enable the interrupt

  // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
  Strobe( RF_SRX );
}

void ReceiveOff(void)
{
  //RF1AIE &= ~BIT9;                          // Disable RX interrupts
  //RF1AIFG &= ~BIT9;                         // Clear pending IFG

  // It is possible that ReceiveOff is called while radio is receiving a packet.
  // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe
  // such that the RXFIFO is empty prior to receiving a packet.
  Strobe( RF_SIDLE );
  Strobe( RF_SFRX  );
}