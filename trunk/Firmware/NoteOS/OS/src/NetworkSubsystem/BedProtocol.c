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

static link_network_bed_header* currentPacket = NULL;
static uint8_t currentPacketLength = 0;

void* BedProtocol_CreatePacket(uint8_t receiver, uint8_t type, uint8_t size)
{
	currentPacket = MemoryManager_Allocate(size);

	if (currentPacket == NULL)
	{
		return NULL;
	}

	currentPacket->link.type = type;
	currentPacket->network.receiver = receiver;
	currentPacket->network.sender = address;

	currentPacketLength = size;

	return currentPacket;
}

bool BedProtocol_QueuePacket()
{
	if (currentPacket == NULL)
	{
		return false;
	}

	return QueueLinkPacket(currentPacket, currentPacketLength);
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
