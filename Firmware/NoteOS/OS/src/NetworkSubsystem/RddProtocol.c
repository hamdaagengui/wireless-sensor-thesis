/*
 * RddProtocol.c
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#include "RddProtocol.h"
#include "../Diagnostics/Diagnostics.h"
#include "../Collections/Queue.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../MemorySubsystem/MemoryManager.h"

static uint8_t transportRddNextSequenceNumber = 0;
static uint8_t transportRddQueue[Queue_CalculateSize(sizeof(queue_element), NETWORK_TRANSPORT_RDD_QUEUE_SIZE)];
static link_network_rdd_header* transportRddCurrentPacket;
static uint8_t transportRddCurrentPacketLength;
static uint8_t transportRddTimer;

void RddProtocol_Initialize()
{
	Queue_Initialize(transportRddQueue, sizeof(queue_element), NETWORK_TRANSPORT_RDD_QUEUE_SIZE);
}

void* RddProtocol_CreatePacket(uint8_t receiver, uint8_t type, uint8_t size)
{
	if (Queue_IsFull(transportRddQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_UNABLE_TO_CREATE_RDD_PACKET);
		return NULL;
	}
	queue_element* qe = Queue_Head(transportRddQueue);

	qe->size = size;
	qe->object = MemoryManager_Allocate(qe->size);

	if (qe->object == NULL)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_UNABLE_TO_CREATE_RDD_PACKET);
		return NULL;
	}

	link_network_rdd_header* p = qe->object;
	p->link.type = type;
	p->network.receiver = receiver;
	p->network.sender = address;
	p->transport.sequenceNumber = transportRddNextSequenceNumber++;

	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_CREATED_RDD_PACKET);

	return qe->object;
}

void RddProtocol_QueuePacket()
{
	Queue_AdvanceHead(transportRddQueue);
}

void RddProtocol_AcknowledgePacket(link_network_rdd_header* packet)
{

}

bool RddProtocol_ProcessPacket(void** data, uint8_t length, block_handler packetHandler)
{
	if (VerifyNetworkLayerHeader(data, length)) // a network layer packet for this node?
	{
		if (EventDispatcher_Process(packetHandler, *data, length)) // add to event queue
		{
			RddProtocol_AcknowledgePacket(*data); // acknowledge packet if it could be added to queue

			*data = NULL; // keep the allocated memory

			return true;
		}
	}

	return false;
}

void RddProtocol_TimeoutHandler()
{
	transportRddTimer = NETWORK_TRANSPORT_RDD_TIMEOUT;
}
