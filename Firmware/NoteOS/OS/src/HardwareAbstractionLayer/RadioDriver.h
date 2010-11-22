#ifndef RADIODRIVER_H_
#define RADIODRIVER_H_

#include "../DefaultConfiguration.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/RadioDriver.h"
#else
#error "No radio driver found for the selected processor!"
#endif

extern const uint8_t radioDriver_TxPower[];

/**
 * Initialize the transceiver and sets the frame receive call back function.
 */
extern void RadioDriver_Initialize();

/**
 *
 */
extern void RadioDriver_Start();

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
 * Same as GetRawRssi but here the value is converted to dBm.
 */
extern int8_t RadioDriver_GetRssi();

/**
 * Send a frame.
 * The transceiver transmission buffer can hold a maximum of 128 bytes including the
 * length specifier and the 16 bit FCS if automatic CRC generation and check is used.
 * This leaves a maximum of either 127 or 125 bytes for user data.
 */
extern void RadioDriver_Send(void* data, uint8_t length);

#endif /* RADIODRIVER_H_ */
