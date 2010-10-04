/*
 * DefaultConfiguration.h
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#ifndef DEFAULTCONFIGURATION_H_
#define DEFAULTCONFIGURATION_H_

// Network subssystem
#define NETWORK_CAN_ROUTE																						1
#define MESSAGE_QUEUE_SIZE																					200

// Event subsystem
#define EVENTDISPATCHER_HIGHEST_EVENT_ID														128
#define EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS								8
#define EVENTDISPATCHER_QUEUE_SIZE																	8
#define EVENTDISPATCHER_REPORT_DATA_SIZE														8
#define EVENTDISPATCHER_REPORT_DATA_POOL_SIZE												16

// Diagnostics
#define DIAGNOSTICSLINK_TRANSMISSION_BUFFER_SIZE										200
#define DIAGNOSTICSLINK_RECEPTION_BUFFER_SIZE												200
#define DIAGNOSTICSLINK_MAXIMUM_FRAME_SIZE													50

// Peripherals

//   SPI
#define SPI_TRANSFER_QUEUE_SIZE																			10

#include <Configuration.h>

#endif /* DEFAULTCONFIGURATION_H_ */
