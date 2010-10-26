/*
 * MemoryManager.c
 *
 *  Created on: 25/10/2010
 *      Author: coma
 */

#include "../Collections/Pool.h"
#include "../DefaultConfiguration.h"

static uint8_t networkPool[Pool_CalculateSize(NETWORK_MAXIMUM_LINK_PACKET_SIZE, NETWORK_LINK_PACKET_POOL_SIZE)];
static uint8_t sensorPool[Pool_CalculateSize(SENSOR_MAXIMUM_REPORT_SIZE, SENSOR_REPORT_POOL_SIZE)];
static const void* const networkPoolStart = networkPool;
static const void* const networkPoolEnd = networkPool + sizeof(networkPool) - 1;
static const void* const sensorPoolStart = sensorPool;
static const void* const sensorPoolEnd = sensorPool + sizeof(sensorPool) - 1;

void MemoryManager_Initialize()
{
	Pool_Initialize(networkPool, NETWORK_LINK_PACKET_POOL_SIZE, NETWORK_LINK_PACKET_POOL_SIZE);
	Pool_Initialize(sensorPool, SENSOR_MAXIMUM_REPORT_SIZE, SENSOR_REPORT_POOL_SIZE);
}

void* MemoryManager_AllocateNetworkBlock()
{
	com(DEBUG_MEMORY_ALLOCATE_NETWORK_BLOCK);
	return Pool_AllocateBlock(networkPool);
}

void MemoryManager_ReleaseNetworkBlock(void* block)
{
	com(DEBUG_MEMORY_RELEASE_NETWORK_BLOCK);
	Pool_ReleaseBlock(networkPool, block);
}

void* MemoryManager_AllocateSensorBlock()
{
	com(DEBUG_MEMORY_ALLOCATE_SENSOR_BLOCK);
	return Pool_AllocateBlock(sensorPool);
}

void MemoryManager_ReleaseSensorBlock(void* block)
{
	com(DEBUG_MEMORY_RELEASE_SENSOR_BLOCK);
	Pool_ReleaseBlock(sensorPool, block);
}

void MemoryManager_ReleaseAnyBlock(void* block)
{
	if (networkPoolStart <= block && block <= networkPoolEnd)
	{
		MemoryManager_ReleaseNetworkBlock(block);
	}
	else if (sensorPoolStart <= block && block <= sensorPoolEnd)
	{
		MemoryManager_ReleaseSensorBlock(block);
	}
	else
	{
		// error - invalid release
	}
}
