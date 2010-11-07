/*
 * MemoryManager.c
 *
 *  Created on: 25/10/2010
 *      Author: coma
 */

#include "../Collections/Pool.h"
#include "../DefaultConfiguration.h"
#include "../Diagnostics/Diagnostics.h"

static uint8_t networkPool[Pool_CalculateSize(NETWORK_MAXIMUM_LINK_PACKET_SIZE, NETWORK_LINK_PACKET_POOL_SIZE)];
static uint8_t sensorPool[Pool_CalculateSize(SENSOR_MAXIMUM_REPORT_SIZE, SENSOR_REPORT_POOL_SIZE)];
static const uint8_t* const networkPoolEnd = networkPool + sizeof(networkPool) - 1;
static const uint8_t* const sensorPoolEnd = sensorPool + sizeof(sensorPool) - 1;

void MemoryManager_Initialize()
{
	Pool_Initialize(networkPool, NETWORK_LINK_PACKET_POOL_SIZE, NETWORK_LINK_PACKET_POOL_SIZE);
	Pool_Initialize(sensorPool, SENSOR_MAXIMUM_REPORT_SIZE, SENSOR_REPORT_POOL_SIZE);
}

void* MemoryManager_AllocateNetworkBlock()
{
	void* b = Pool_AllocateBlock(networkPool);

	if (b == NULL)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_FAILED_TO_ALLOCATE_NETWORK_BLOCK);
	}
	else
	{
		Diagnostics_SendEvent(DIAGNOSTICS_ALLOCATED_NETWORK_BLOCK);
	}

	return b;
}

void MemoryManager_ReleaseNetworkBlock(void* block)
{
	Diagnostics_SendEvent(DIAGNOSTICS_RELEASED_NETWORK_BLOCK);
	Pool_ReleaseBlock(networkPool, block);
}

void* MemoryManager_AllocateSensorBlock()
{
	return Pool_AllocateBlock(sensorPool);
}

void MemoryManager_ReleaseSensorBlock(void* block)
{
	Pool_ReleaseBlock(sensorPool, block);
}

void MemoryManager_ReleaseAnyBlock(void* block)
{
	uint8_t* b = block;

	if (networkPool <= b && b <= networkPoolEnd)
	{
		MemoryManager_ReleaseNetworkBlock(block);
	}
	else if (sensorPool <= b && b <= sensorPoolEnd)
	{
		MemoryManager_ReleaseSensorBlock(block);
	}
	else
	{
		// error - invalid release
	}
}
