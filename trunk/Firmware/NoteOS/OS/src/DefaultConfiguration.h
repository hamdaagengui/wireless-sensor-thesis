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
#define NETWORK_LINK_QUEUE_SIZE																			10
#define NETWORK_TRANSPORT_QUEUE_SIZE																10
#define NETWORK_TRANSPORT_TIMEOUT																		64 // in units of: 1 / (NETWORK_TIMER_FREQUENCY / NETWORK_COMMUNICATION_SLOT_FREQUENCY) (default = 500 ms)
#define NETWORK_TIMER_FREQUENCY																			32768 // In Hz
#define NETWORK_COMMUNICATION_SLOT_FREQUENCY												128 // Number of transfers per second (i.e. RTS+CTS+DATA+ACK)
#define NETWORK_NUMBER_OF_RTS_SLOTS																	6
#define NETWORK_RTS_SLOT_DURATION																		100 // Duration of an RTS slot in micro seconds)

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
#define DIAGNOSTICS_LINK_TRANSMISSION_BUFFER_SIZE										150
#define DIAGNOSTICS_LINK_RECEPTION_BUFFER_SIZE											30
#define DIAGNOSTICS_LINK_BAUDRATE																		500000

// Peripherals

//   SPI
#define SPI_TRANSFER_QUEUE_SIZE																			10

#include <Configuration.h>

#endif /* DEFAULTCONFIGURATION_H_ */
