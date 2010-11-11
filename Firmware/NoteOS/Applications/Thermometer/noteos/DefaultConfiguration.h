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
#define NETWORK_MAXIMUM_LINK_PACKET_SIZE														64
#define NETWORK_LINK_PACKET_POOL_SIZE																10
#define NETWORK_TIMER_FREQUENCY																			32768 // In Hz
#define NETWORK_TRANSFER_WINDOW_FREQUENCY														20 // Number of transfer windows per second (i.e. RTS+CTS+DATA+ACK)
#define NETWORK_NUMBER_OF_RTS_SLOTS																	6

// Sensor subsystem
#define SENSOR_MAXIMUM_REPORT_SIZE																	8
#define SENSOR_REPORT_POOL_SIZE																			16

// Event subsystem
#define EVENTDISPATCHER_HIGHEST_EVENT_ID														128
#define EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS								8
#define EVENTDISPATCHER_QUEUE_SIZE																	8

// Memory subsystem
#define POOL_1_ALLOCATION_SIZE																			8
#define POOL_1_ALLOCATION_COUNT																			10
#define POOL_2_ALLOCATION_SIZE																			16
#define POOL_2_ALLOCATION_COUNT																			10
#define POOL_3_ALLOCATION_SIZE																			32
#define POOL_3_ALLOCATION_COUNT																			10
#define POOL_4_ALLOCATION_SIZE																			64
#define POOL_4_ALLOCATION_COUNT																			10

// Diagnostics
#define DIAGNOSTICS_LINK_TRANSMISSION_BUFFER_SIZE										100
#define DIAGNOSTICS_LINK_RECEPTION_BUFFER_SIZE											50

// Peripherals

//   SPI
#define SPI_TRANSFER_QUEUE_SIZE																			10

#include <Configuration.h>

#endif /* DEFAULTCONFIGURATION_H_ */
