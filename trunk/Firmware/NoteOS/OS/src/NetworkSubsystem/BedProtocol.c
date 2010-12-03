/*
 * BedProtocol.c
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#include "BedProtocol.h"
#include "NetworkInternals.h"
#include "../Diagnostics/Diagnostics.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../MemorySubsystem/MemoryManager.h"

static link_network_bed_transport_header* transportBedCurrentPacket;
static uint8_t transportBedCurrentPacketLength;

void* BedProtocol_CreatePacket(uint8_t receiver, uint8_t type, uint8_t size)
{
	link_network_bed_transport_header* p = MemoryManager_Allocate(size);

	if (p == NULL)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_UNABLE_TO_CREATE_BED_PACKET);
		return NULL;
	}

	p->link.type = type;
	p->network.receiver = receiver;
	p->network.sender = address;

	transportBedCurrentPacket = p;
	transportBedCurrentPacketLength = size;

	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_CREATED_BED_PACKET);

	return p;
}

bool BedProtocol_QueuePacket()
{
	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_SENT_BED_PACKET);
	return QueueLinkPacket((link_network_header*) transportBedCurrentPacket, transportBedCurrentPacketLength);
}

bool BedProtocol_ProcessPacket(void** data, uint8_t length, block_handler packetHandler)
{
	if (VerifyNetworkLayerHeader(data, length)) // a network layer packet for this node?
	{
		if (EventDispatcher_Process(packetHandler, *data, length)) // add to event queue
		{
			*data = NULL; // keep the allocated memory

			return true;
		}
	}

	return false;
}
