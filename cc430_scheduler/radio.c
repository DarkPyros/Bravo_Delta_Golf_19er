/*
 * radio.c
 *
 *  Created on: Sep 16, 2013
 *      Author: Chris
 */


// Rf settings for CC430
RF_SETTINGS code rfSettings = {
    0x06,  // IOCFG0        GDO0 Output Configuration
    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0x05,  // PKTCTRL0      Packet Automation Control
    0x7F,  // CHANNR        Channel Number
    0x06,  // FSCTRL1       Frequency Synthesizer Control
    0x22,  // FREQ2         Frequency Control Word, High Byte
    0xB4,  // FREQ1         Frequency Control Word, Middle Byte
    0x2F,  // FREQ0         Frequency Control Word, Low Byte
    0xCA,  // MDMCFG4       Modem Configuration
    0x83,  // MDMCFG3       Modem Configuration
    0x13,  // MDMCFG2       Modem Configuration
    0x35,  // DEVIATN       Modem Deviation Setting
    0x10,  // MCSM0         Main Radio Control State Machine Configuration
    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
    0x43,  // AGCCTRL2      AGC Control
    0xFB,  // WORCTRL       Wake On Radio Control
    0xE9,  // FSCAL3        Frequency Synthesizer Calibration
    0x2A,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1F,  // FSCAL0        Frequency Synthesizer Calibration
    0x81,  // TEST2         Various Test Settings
    0x35,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};
