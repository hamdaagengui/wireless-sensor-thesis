/*
 * Diagnostics.h
 *
 *  Created on: 05/11/2010
 *      Author: Coma
 */

#ifndef DIAGNOSTICS_H_
#define DIAGNOSTICS_H_

#include "../Globals.h"
#include <stdio.h>

typedef enum
{
	// Kernel
	DIAGNOSTICS_SYSTEM_INITIALIZED,


	// Event dispatcher
	DIAGNOSTICS_EVENT_DISPATCHER_COMPLETION_QUEUED,
	DIAGNOSTICS_EVENT_DISPATCHER_COMPLETION_EXECUTING,
	DIAGNOSTICS_EVENT_DISPATCHER_PROCESS_QUEUED,
	DIAGNOSTICS_EVENT_DISPATCHER_PROCESS_EXECUTING,
	DIAGNOSTICS_EVENT_DISPATCHER_PUBLISH_QUEUED,
	DIAGNOSTICS_EVENT_DISPATCHER_PUBLISH_EXECUTING,
	DIAGNOSTICS_EVENT_DISPATCHER_QUEUE_OVERFLOW,
	DIAGNOSTICS_EVENT_DISPATCHER_TOO_MANY_EVENT_SUBSCRIBERS,


	// Timer
	DIAGNOSTICS_TIMER_CONFIGURATION_CREATED,
	DIAGNOSTICS_TIMER_STARTED,
	DIAGNOSTICS_TIMER_NO_FREE_TIMER_OBJECTS,
	DIAGNOSTICS_TIMER_RESTARTED,
	DIAGNOSTICS_TIMER_STOPPED,
	DIAGNOSTICS_TIMER_FIRED_AND_CONTINUED,
	DIAGNOSTICS_TIMER_FIRED_AND_STOPPED,


	// Memory subsystem
	DIAGNOSTICS_POOL_1_ALLOCATION,
	DIAGNOSTICS_POOL_1_RELEASE,
	DIAGNOSTICS_POOL_1_EXHAUSTED,
	DIAGNOSTICS_POOL_2_ALLOCATION,
	DIAGNOSTICS_POOL_2_RELEASE,
	DIAGNOSTICS_POOL_2_EXHAUSTED,
	DIAGNOSTICS_POOL_3_ALLOCATION,
	DIAGNOSTICS_POOL_3_RELEASE,
	DIAGNOSTICS_POOL_3_EXHAUSTED,
	DIAGNOSTICS_POOL_4_ALLOCATION,
	DIAGNOSTICS_POOL_4_RELEASE,
	DIAGNOSTICS_POOL_4_EXHAUSTED,
	DIAGNOSTICS_MEMORY_ALLOCATION_FAILED,
	DIAGNOSTICS_MEMORY_RELEASE_FAILED,


	// Network subsystem
	DIAGNOSTICS_NETWORK_LINK_RTS_SENT,
	DIAGNOSTICS_NETWORK_LINK_RTS_RECEIVED,
	DIAGNOSTICS_NETWORK_LINK_CTS_SENT,
	DIAGNOSTICS_NETWORK_LINK_CTS_RECEIVED,
	DIAGNOSTICS_NETWORK_LINK_DATA_SENT,
	DIAGNOSTICS_NETWORK_LINK_DATA_RECEIVED,
	DIAGNOSTICS_NETWORK_LINK_ACK_SENT,
	DIAGNOSTICS_NETWORK_LINK_ACK_RECEIVED,
	DIAGNOSTICS_NETWORK_LINK_BROADCAST_SENT,
	DIAGNOSTICS_NETWORK_LINK_BROADCAST_RECEIVED,

	DIAGNOSTICS_DATA_QUEUED,
	DIAGNOSTICS_DATA_NOT_QUEUED,
	DIAGNOSTICS_NETWORK_TICK,
	DIAGNOSTICS_NETWORK_FORWARDING,

	DIAGNOSTICS_NETWORK_LINK_PACKET_EXPECTED,


	// NEW
	DIAGNOSTICS_TIMER_TICK = 62,
	// NEW ADC
	DIAGNOSTICS_ADC_CREATED,
	DIAGNOSTICS_ADC_CONVERSION_QUEUED,
	DIAGNOSTICS_ADC_FAILED_TO_QUEUE_CONVERSION,
	DIAGNOSTICS_ADC_CONVERSION_STARTED,
	DIAGNOSTICS_ADC_CONVERSION_COMPLETED,

	DIAGNOSTICS_NETWORK_FORWARDING_DROPPED, // not used anymore

	DIAGNOSTICS_POWER_MANAGER_RESOURCE_REQUEST_OVERFLOW,
	DIAGNOSTICS_POWER_MANAGER_RESOURCE_RELEASE_OVERFLOW,

	DIAGNOSTICS_NETWORK_LINK_PACKET_QUEUED,
	DIAGNOSTICS_NETWORK_LINK_QUEUE_OVERRUN,

	DIAGNOSTICS_NETWORK_UNABLE_TO_CREATE_BED_PACKET,
	DIAGNOSTICS_NETWORK_UNABLE_TO_CREATE_RDD_PACKET,

	DIAGNOSTICS_NETWORK_CREATED_BED_PACKET,
	DIAGNOSTICS_NETWORK_CREATED_RDD_PACKET,

	DIAGNOSTICS_NETWORK_SENT_BED_PACKET,
	DIAGNOSTICS_NETWORK_SENT_RDD_PACKET,

	DIAGNOSTICS_NETWORK_APPLICATION_JOINED,
	DIAGNOSTICS_NETWORK_APPLICATION_JOIN_REQUEST_SENT,
	DIAGNOSTICS_NETWORK_CREATED_JOIN_RESPONSE,
	DIAGNOSTICS_NETWORK_GOT_JOIN_REQUEST,
	DIAGNOSTICS_NETWORK_GOT_JOIN_RESPONSE,

	DIAGNOSTICS_MASTER_NODE_RUNNING,
	DIAGNOSTICS_SLAVE_NODE_RUNNING,

	DIAGNOSTICS_NETWORK_BED_RAW_DATA_RECEIVED,

	DIAGNOSTICS_NETWORK_LINK_TX_PACKET_DROPPED,

	DIAGNOSTICS_NETWORK_LINK_QUEUED_DIRECT_PACKET, // dead
	DIAGNOSTICS_NETWORK_LINK_QUEUED_MESH_PACKET,
	DIAGNOSTICS_NETWORK_LINK_QUEUED_STAR_PACKET,
} diagnostics_event;

#if DIAGNOSTICS == 1
#define print(text, ...) { char __text[200]; sprintf(__text, text, ## __VA_ARGS__); Diagnostics_SendMessage(__text); }
#else
#define print(text, ...)
#endif

extern void Diagnostics_Initialize();
extern void Diagnostics_SendEvent(uint8_t event);
extern void Diagnostics_SendMessage(char* text);
extern void Diagnostics_SendGraph(int8_t value);
//extern void Diagnostics_SendState(uint8_t stateMachineId, uint8_t newState);

#endif /* DIAGNOSTICS_H_ */
