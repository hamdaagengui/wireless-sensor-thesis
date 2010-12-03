/*
 * Configuration.h
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

// General
#define MASTER_NODE																									0

// Board Support Package selection
#define BOARDSUPPORTPACKAGE_STK600_ATMEGA128RFA1										1

// Network subsystem
#define NETWORK_LINK_TIMER_FREQUENCY																32768 // In Hz
#define NETWORK_LINK_COMMUNICATION_SLOT_FREQUENCY										128 // Number of transfers per second (i.e. RTS + CTS + DATA + ACK)
#define NETWORK_LINK_NUMBER_OF_RTS_GUARD_SLOTS											4
#define NETWORK_LINK_RTS_GUARD_SLOT_DURATION												256 // Duration of an RTS guard slot in micro seconds)
#define NETWORK_LINK_CCA_SAMPLE_COUNT																8
#define NETWORK_LINK_CCA_ALPHA																			0.06
#define NETWORK_LINK_CCA_OUTLIER_COUNT															5
#define NETWORK_LINK_CCA_THRESHOLD_SAFE_BAND												5
#define NETWORK_LINK_CCA_CHECK_INTERVAL															2
#define NETWORK_LINK_CCA_CHECK_DURATION															(NETWORK_LINK_CCA_OUTLIER_COUNT * NETWORK_LINK_CCA_CHECK_INTERVAL)
#define NETWORK_LINK_MAXIMUM_TRANSMISSION_ATTEMPTS									5
#define NETWORK_LINK_MAXIMUM_PACKET_SIZE														64
#define NETWORK_LINK_QUEUE_SIZE																			10
#define NETWORK_NETWORK_CAN_ROUTE																		1
#define NETWORK_TRANSPORT_RDD_QUEUE_SIZE														8
#define NETWORK_TRANSPORT_RDD_TIMEOUT																64 // in units of: 1 / (NETWORK_TIMER_FREQUENCY / NETWORK_COMMUNICATION_SLOT_FREQUENCY) (default = 500 ms)
//
// Sensor subsystem
#define SENSOR_MANAGER_MAXIMUM_NUMBER_OF_SENSORS										8

// Event subsystem
#define EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS								8
#define EVENTDISPATCHER_QUEUE_SIZE																	8

// Timer
#define TIMER_MAXIMUM_NUMBER_OF_TIMERS															10

// Memory subsystem
#define MEMORYMANAGER_POOL_1_ALLOCATION_SIZE												8
#define MEMORYMANAGER_POOL_1_ALLOCATION_COUNT												10
#define MEMORYMANAGER_POOL_2_ALLOCATION_SIZE												16
#define MEMORYMANAGER_POOL_2_ALLOCATION_COUNT												10
#define MEMORYMANAGER_POOL_3_ALLOCATION_SIZE												32
#define MEMORYMANAGER_POOL_3_ALLOCATION_COUNT												10
#define MEMORYMANAGER_POOL_4_ALLOCATION_SIZE												64
#define MEMORYMANAGER_POOL_4_ALLOCATION_COUNT												10

// Peripherals

//   SPI
#define SPI_OPERATION_QUEUE_SIZE																		10

//   ADC
#define ADC_OPERATION_QUEUE_SIZE																		10

// Diagnostics
#define DIAGNOSTICS																									1
#define DIAGNOSTICS_LINK_TRANSMISSION_BUFFER_SIZE										200
#define DIAGNOSTICS_LINK_RECEPTION_BUFFER_SIZE											10
#define DIAGNOSTICS_LINK_BAUDRATE																		1000000

#endif /* CONFIGURATION_H_ */
