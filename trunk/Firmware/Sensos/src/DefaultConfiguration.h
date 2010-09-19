/*
 * DefaultConfiguration.h
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#ifndef DEFAULTCONFIGURATION_H_
#define DEFAULTCONFIGURATION_H_

#define PDD_MAX_EVENTS													32
#define PDD_MAX_SUBSCRIBERS											8
#define PDD_QUEUE_SIZE                          8
#define PDD_DETAILS_OBJECT_COUNT								16
#define PDD_DETAILS_OBJECT_SIZE									8

#define NETWORK_CAN_ROUTE												1

// Event subsystem
#define EVENTDISPATCHER_HIGHEST_EVENT_ID														128
#define EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS								8
#define EVENTDISPATCHER_QUEUE_SIZE																	8
#define EVENTDISPATCHER_REPORT_DATA_SIZE														8
#define EVENTDISPATCHER_REPORT_DATA_POOL_SIZE												16

// Peripherals

//   SPI
#define SPI_TRANSFER_QUEUE_SIZE																			10

#include <Configuration.h>

#endif /* DEFAULTCONFIGURATION_H_ */
