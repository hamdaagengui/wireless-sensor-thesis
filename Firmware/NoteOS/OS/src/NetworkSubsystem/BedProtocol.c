/*
 * BedProtocol.c
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#include "BedProtocol.h"
#include "NetworkInternals.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../MemorySubsystem/MemoryManager.h"

static link_network_bed_header* transportBedCurrentPacket = NULL;
static uint8_t transportBedCurrentPacketLength = 0;

void* BedProtocol_CreatePacket(uint8_t receiver, uint8_t type, uint8_t size)
{
	transportBedCurrentPacket = MemoryManager_Allocate(size);

	if (transportBedCurrentPacket == NULL)
	{
		return NULL;
	}

	transportBedCurrentPacket->link.type = type;
	transportBedCurrentPacket->network.receiver = receiver;
	transportBedCurrentPacket->network.sender = address;
	transportBedCurrentPacketLength = size;

	return transportBedCurrentPacket;
}

bool BedProtocol_QueuePacket()
{
	if (transportBedCurrentPacket == NULL)
	{
		return false;
	}

	return QueueLinkPacket(transportBedCurrentPacket, transportBedCurrentPacketLength);
}

void BedProtocol_ProcessPacket(void** data, uint8_t length, block_handler packetHandler)
{
	if (VerifyNetworkLayerHeader(data, length)) // a network layer packet for this node?
	{
		if (EventDispatcher_Process(packetHandler, *data, length)) // add to event queue
		{
			*data = NULL; // keep the allocated memory
		}
	}
}
