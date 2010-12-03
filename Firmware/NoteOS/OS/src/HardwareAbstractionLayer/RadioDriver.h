#ifndef RADIODRIVER_H_
#define RADIODRIVER_H_

#include "../Globals.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/RadioDriver.h"
#else
#define RADIODRIVER_UNIMPLEMENTED
#endif

#ifndef RADIODRIVER_UNIMPLEMENTED

extern const uint8_t radioDriver_TxPower[];

/**
 * Initialize the transceiver and sets the frame receive call back function.
 */
extern void RadioDriver_Initialize();

/**
 *
 */
extern void RadioDriver_SetFrameReceivedHandler(bidirectional_block_handler handler);

/**
 * Set the transmit power of the transceiver.
 * Use on of the RADIODRIVER_TX_POWER_X_DBM constants.
 */
extern void RadioDriver_SetTxPower(uint8_t power);

/**
 * Set the transceiver channel.
 * Use either RADIODRIVER_CHANNEL_X, RADIODRIVER_FREQUENCY_X_MHZ or just an integer between 11 and 26 (both included).
 */
extern void RadioDriver_SetChannel(uint8_t channel);

/**
 * Get the current RSSI in dBm.
 */
extern int8_t RadioDriver_GetRssi();

/**
 * Get the RSSI (in dBm) as it was when the frame header was received.
 */
extern int8_t RadioDriver_GetRssiAtFrameStart();

/**
 * Send a frame.
 * The transceiver transmission buffer can hold a maximum of 128 bytes including the
 * length specifier and the 16 bit FCS if automatic CRC generation and check is used.
 * This leaves a maximum of either 127 or 125 bytes for user data.
 */
extern void RadioDriver_Send(void* data, uint8_t length);

/**
 * Switch to receive mode.
 * Only relevant in non auto rx mode! See configuration define RADIODRIVER_AUTO_RX.
 * It's safe to call this function even if the transceiver is already in receive mode.
 * When a frame has been received the transmitter switches back to non rx mode and
 * EnableReceiveMode must be called again to receive a new frame.
 */
extern void RadioDriver_EnableReceiveMode();

/**
 * Switch to idle mode.
 * Only relevant in non auto rx mode! See configuration define RADIODRIVER_AUTO_RX.
 * It's safe to call this function even if the transceiver is already in idle mode.
 * When a frame is received an action equivalent to calling this function is performed.
 */
extern void RadioDriver_DisableReceiveMode();

#endif

#endif /* RADIODRIVER_H_ */
