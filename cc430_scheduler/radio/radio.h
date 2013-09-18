#ifndef RADIO_H_
#define RADIO_H_

#include "../RF1A/RF1A.h"

// Product = CC430Fx13x
// Chip version = C   (PG 0.7)
// Crystal accuracy = 10 ppm
// X-tal frequency = 26 MHz
// RF output power = 0 dBm
// RX filter bandwidth = 101.562500 kHz
// Deviation = 20.629883 kHz
// Data Rate = 38.3835 kBaud
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
    0x06,	// FSCTRL1   Frequency Synthesizer Control
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x22,   // FREQ2     Frequency Control Word, High Byte
    0xB4,   // FREQ1     Frequency Control Word, Middle Byte
    0x2F,   // FREQ0     Frequency Control Word, Low Byte
    0xCA,   // MDMCFG4   Modem Configuration
    0x83,   // MDMCFG3   Modem Configuration
    0x13,   // MDMCFG2   Modem Configuration
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.
    0x00,   // CHANNR    Channel number.
    0x35,   // DEVIATN   Modem Deviation Setting
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end TX configuration.
    0x10,   // MCSM0     Main Radio Control State Machine Configuration
    0x16,   // FOCCFG    Frequency Offset Compensation Configuration
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x43,   // AGCCTRL2  AGC Control
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.
	0x80,   // WOREVT1
	0x00,   // WOREVT0
	0xFB,   // WORCTRL
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
    0x06,   // IOCFG0    GDO0 output pin configuration. Refer to SmartRF® Studio User Manual for detailed pseudo register explanation.
    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x30    // PKTLEN    Packet length.
};

#define PATABLE_VAL (0x51)

void Radio_Init (void);

#endif /* RADIO_H_ */
