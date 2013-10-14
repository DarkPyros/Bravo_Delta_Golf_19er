/*------------------------------------------------------------------*-
  RF1A.H (v1.00)
  ------------------------------------------------------------------

  Header file for low-level access to the CC430 radio module

-*------------------------------------------------------------------*/

#ifndef RF1A_H_
#define RF1A_H_

#include "../main.h"

typedef struct S_RF_SETTINGS {
    tByte fsctrl1;   // Frequency synthesizer control.
    tByte fsctrl0;   // Frequency synthesizer control.
    tByte freq2;     // Frequency control word, high byte.
    tByte freq1;     // Frequency control word, middle byte.
    tByte freq0;     // Frequency control word, low byte.
    tByte mdmcfg4;   // Modem configuration.
    tByte mdmcfg3;   // Modem configuration.
    tByte mdmcfg2;   // Modem configuration.
    tByte mdmcfg1;   // Modem configuration.
    tByte mdmcfg0;   // Modem configuration.
    tByte channr;    // Channel number.
    tByte deviatn;   // Modem deviation setting (when FSK modulation is enabled).
    tByte frend1;    // Front end RX configuration.
    tByte frend0;    // Front end RX configuration.
    tByte mcsm0;     // Main Radio Control State Machine configuration.
    tByte mcsm1;     // Main Radio Control State Machine configuration.
    tByte foccfg;    // Frequency Offset Compensation Configuration.
    tByte bscfg;     // Bit synchronization Configuration.
    tByte agcctrl2;  // AGC control.
    tByte agcctrl1;  // AGC control.
    tByte agcctrl0;  // AGC control.
    tByte worevt1;   //
    tByte worevt0;   //
    tByte worctrl;   //
    tByte fscal3;    // Frequency synthesizer calibration.
    tByte fscal2;    // Frequency synthesizer calibration.
    tByte fscal1;    // Frequency synthesizer calibration.
    tByte fscal0;    // Frequency synthesizer calibration.
    tByte fstest;    // Frequency synthesizer calibration control
    tByte test2;     // Various test settings.
    tByte test1;     // Various test settings.
    tByte test0;     // Various test settings.
    tByte fifothr;   // RXFIFO and TXFIFO thresholds.
    tByte iocfg2;    // GDO2 output pin configuration
    tByte iocfg0;    // GDO0 output pin configuration
    tByte pktctrl1;  // Packet automation control.
    tByte pktctrl0;  // Packet automation control.
    tByte addr;      // Device address.
    tByte pktlen;    // Packet length.
} RF_SETTINGS;

void ResetRadioCore (void);
tByte Strobe(tByte strobe);

void WriteRfSettings(const RF_SETTINGS *pRfSettings);

void WriteSingleReg(tByte addr, tByte value);
void WriteBurstReg(tByte addr, tByte const * const buffer, tByte count);
tByte ReadSingleReg(tByte addr);
void ReadBurstReg(tByte addr, tByte * const buffer, tByte count);
void WriteSinglePATable(tByte value);
void WriteBurstPATable(tByte const * const buffer, tByte count);

#endif /* RF1A_H_ */
