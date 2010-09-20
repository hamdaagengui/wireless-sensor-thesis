/*
 *	Module Name
 *
 *		RadioDriver
 *
 *
 *	Programmer
 *
 *		Claus Andersen
 *
 *
 *	Description
 *
 *		This is a driver for the ATmega128RFA1 build in RF transceiver.
 *
 *
 *	Configuration
 *
 *		Part of the configuration of the radio driver is done using compile time conditional compilation to
 *		save code space and to increase speed. To use these use #defines in the Config.h file in your project.
 *
 *		*----------------------*--------*-------------------------------------------------------------------*
 *		| Name                 | Values | Effect                                                            |
 *		*----------------------*--------*-------------------------------------------------------------------*
 *		| RADIODRIVER_AUTO_RX  | (none) | If defined the transceiver will always be in the RX_ON mode when  |
 *		|                      |        | not transmitting. This enables easy usage of the driver but       |
 *		|                      |        | prevents low power mode of the transceiver.                       |
 *		*----------------------*--------*-------------------------------------------------------------------*
 *		| RADIODRIVER_USE_CRC  | (none) | Defining RADIODRIVER_USE_CRC enables the transceivers build in    |
 *		|                      |        | CRC generation and check. Received frames that fail the CRC       |
 *		|                      |        | check are silently discarded.                                     |
 *		*----------------------*--------*-------------------------------------------------------------------*
 *
 *
 *	Version log
 *
 *		*---------*--------------*----------*---------------------------------------------------------------*
 *		| Version | Date         | Initials | Changes                                                       |
 *		*---------*--------------*----------*---------------------------------------------------------------*
 *		| 1.00.00 | May 2. 2010  | CA       | First version implemented. Only basic mode is supported       |
 *		|         |              |          | though the automatic Frame Check Sequence generator and       |
 *		|         |              |          | checker can be enabled.                                       |
 *		*---------*--------------*----------*---------------------------------------------------------------*
 *
 */

#include "Globals.h"

enum
{
	RADIODRIVER_BITRATE_250_KBPS, // Default
	RADIODRIVER_BITRATE_500_KBPS,
	RADIODRIVER_BITRATE_1000_KBPS,
	RADIODRIVER_BITRATE_2000_KBPS
};

enum
{
	RADIODRIVER_TX_POWER_3_0_DBM, // Default
	RADIODRIVER_TX_POWER_2_8_DBM,
	RADIODRIVER_TX_POWER_2_3_DBM,
	RADIODRIVER_TX_POWER_1_8_DBM,
	RADIODRIVER_TX_POWER_1_3_DBM,
	RADIODRIVER_TX_POWER_0_7_DBM,
	RADIODRIVER_TX_POWER_0_DBM,
	RADIODRIVER_TX_POWER_MINUS_1_DBM,
	RADIODRIVER_TX_POWER_MINUS_2_DBM,
	RADIODRIVER_TX_POWER_MINUS_3_DBM,
	RADIODRIVER_TX_POWER_MINUS_4_DBM,
	RADIODRIVER_TX_POWER_MINUS_5_DBM,
	RADIODRIVER_TX_POWER_MINUS_7_DBM,
	RADIODRIVER_TX_POWER_MINUS_9_DBM,
	RADIODRIVER_TX_POWER_MINUS_12_DBM,
	RADIODRIVER_TX_POWER_MINUS_17_DBM
};

enum
{
	RADIODRIVER_TX_POWER_MAXIMUM = RADIODRIVER_TX_POWER_3_0_DBM,
	RADIODRIVER_TX_POWER_MINIMUM = RADIODRIVER_TX_POWER_MINUS_17_DBM
};

enum
{
	RADIODRIVER_CHANNEL_11 = 11, // Default
	RADIODRIVER_CHANNEL_12 = 12,
	RADIODRIVER_CHANNEL_13 = 13,
	RADIODRIVER_CHANNEL_14 = 14,
	RADIODRIVER_CHANNEL_15 = 15,
	RADIODRIVER_CHANNEL_16 = 16,
	RADIODRIVER_CHANNEL_17 = 17,
	RADIODRIVER_CHANNEL_18 = 18,
	RADIODRIVER_CHANNEL_19 = 19,
	RADIODRIVER_CHANNEL_20 = 20,
	RADIODRIVER_CHANNEL_21 = 21,
	RADIODRIVER_CHANNEL_22 = 22,
	RADIODRIVER_CHANNEL_23 = 23,
	RADIODRIVER_CHANNEL_24 = 24,
	RADIODRIVER_CHANNEL_25 = 25,
	RADIODRIVER_CHANNEL_26 = 26
};

enum
{
	RADIODRIVER_FREQUENCY_2405_MHZ = RADIODRIVER_CHANNEL_11, // Default
	RADIODRIVER_FREQUENCY_2410_MHZ = RADIODRIVER_CHANNEL_12,
	RADIODRIVER_FREQUENCY_2415_MHZ = RADIODRIVER_CHANNEL_13,
	RADIODRIVER_FREQUENCY_2420_MHZ = RADIODRIVER_CHANNEL_14,
	RADIODRIVER_FREQUENCY_2425_MHZ = RADIODRIVER_CHANNEL_15,
	RADIODRIVER_FREQUENCY_2430_MHZ = RADIODRIVER_CHANNEL_16,
	RADIODRIVER_FREQUENCY_2435_MHZ = RADIODRIVER_CHANNEL_17,
	RADIODRIVER_FREQUENCY_2440_MHZ = RADIODRIVER_CHANNEL_18,
	RADIODRIVER_FREQUENCY_2445_MHZ = RADIODRIVER_CHANNEL_19,
	RADIODRIVER_FREQUENCY_2450_MHZ = RADIODRIVER_CHANNEL_20,
	RADIODRIVER_FREQUENCY_2455_MHZ = RADIODRIVER_CHANNEL_21,
	RADIODRIVER_FREQUENCY_2460_MHZ = RADIODRIVER_CHANNEL_22,
	RADIODRIVER_FREQUENCY_2465_MHZ = RADIODRIVER_CHANNEL_23,
	RADIODRIVER_FREQUENCY_2470_MHZ = RADIODRIVER_CHANNEL_24,
	RADIODRIVER_FREQUENCY_2475_MHZ = RADIODRIVER_CHANNEL_25,
	RADIODRIVER_FREQUENCY_2480_MHZ = RADIODRIVER_CHANNEL_26,
};

enum
{
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_DISABLE, // Default
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_90_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_87_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_84_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_81_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_78_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_75_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_72_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_69_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_66_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_63_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_60_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_57_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_54_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_51_DBM,
	RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_MINUS_48_DBM,
};

/**
 * Set transceiver bit rate.
 * Use on of the RADIODRIVER_BITRATE_<bitrate>_KBPS constants.
 */
extern void RadioDriver_SetBitRate(uint8_t bitRate);

/**
 * Set the threshold below which to ignore frames.
 * Use one of the RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_X_DBM constants.
 * Default value is to receive all frames regardless of RSSI level of their header.
 */
extern void RadioDriver_SetReceiverSensitivityThreshold(uint8_t level);

/**
 * Return a 2 bit random number.
 * A new value is available every 2 us.
 */
extern uint8_t RadioDriver_GetRandomNumber();

/**
 * Starts a calibration cycle of the three things that need calibration.
 * See data sheet pages 120 - 122 for details.
 */
extern void RadioDriver_Calibrate();

/**
 * Waits for a previously initiated send operation to complete.
 */
extern void RadioDriver_WaitForSendToComplete();

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

#ifdef STATISTICS

typedef struct
{
	uint32_t framesReceived;
	uint32_t framesDiscarded;
	uint32_t framesSent;
	int8_t maximumRawRssi;
	int8_t minimumRawRssi;
} radioDriverStatistics_t;

extern radioDriverStatistics_t radioDriverStatistics;

#endif
