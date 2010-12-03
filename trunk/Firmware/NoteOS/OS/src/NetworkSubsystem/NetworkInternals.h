/*
 * NetworkInternals.h
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#ifndef NETWORKINTERNALS_H_
#define NETWORKINTERNALS_H_

#include "Network.h"
#include "BedProtocol.h"
#include "RddProtocol.h"
#include "RoutingLogic.h"

//  Constants
#define GATEWAY_ADDRESS																							0
#define BROADCAST_ADDRESS																						15

typedef enum
{
	LINK_STATE_UNSYNCHRONIZED,
	LINK_STATE_IDLE,
	LINK_STATE_EXPECTING_RTS,
	LINK_STATE_EXPECTING_CTS,
	LINK_STATE_EXPECTING_ACK,
	LINK_STATE_EXPECTING_DATA
} link_state;

typedef enum
{
	CONNECTION_STATE_UNCONNECTED,
	CONNECTION_STATE_CONNECTING,
	CONNECTION_STATE_CONNECTED
} connection_state;

// possible packet types
typedef enum
{
	// link packets
	TYPE_LINK_RTS,
	TYPE_LINK_CTS,
	TYPE_LINK_ACK,
	// network packets
	TYPE_NETWORK_ROUTES,
	// transport packets
	TYPE_TRANSPORT_RDD_ACK,
	// application packets
	//   BED based
	TYPE_APPLICATION_JOIN_REQUEST,
	TYPE_APPLICATION_JOIN_RESPONSE,
	TYPE_APPLICATION_RAW_BED,
	//   RDD based
	TYPE_APPLICATION_RAW_RDD,
	TYPE_APPLICATION_SENSOR_DATA,
	TYPE_APPLICATION_SET_PROPERTY_REQUEST,
	TYPE_APPLICATION_SET_PROPERTY_RESPONSE,
	TYPE_APPLICATION_GET_PROPERTY_REQUEST,
	TYPE_APPLICATION_GET_PROPERTY_RESPONSE,
// future packet types
//	TYPE_READ,
//	TYPE_READ_COMPLETE,
//	TYPE_WRITE,
//	TYPE_WRITE_COMPLETE
//	TYPE_LOAD,
//	TYPE_LOAD_COMPLETE,
//	TYPE_STORE,
//	TYPE_STORE_COMPLETE
} packet_type;

#if MASTER_NODE == 1

extern volatile uint8_t linkState;
extern uint8_t address;

#else

extern volatile uint8_t linkState;
extern uint8_t address;
extern connection_state connectionState;

#endif

extern bool QueueLinkPacket(void* packet, uint8_t length);
extern bool VerifyNetworkLayerHeader(void** data, uint8_t length);

#endif /* NETWORKINTERNALS_H_ */
