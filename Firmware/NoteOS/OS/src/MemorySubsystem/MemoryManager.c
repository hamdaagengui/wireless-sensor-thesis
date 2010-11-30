/*
 * MemoryManager.c
 *
 *  Created on: 25/10/2010
 *      Author: coma
 */

#include "../Collections/Pool.h"
#include "../Diagnostics/Diagnostics.h"

static uint8_t pool1[Pool_CalculateSize(MEMORYMANAGER_POOL_1_ALLOCATION_SIZE, MEMORYMANAGER_POOL_1_ALLOCATION_COUNT)];
static uint8_t pool2[Pool_CalculateSize(MEMORYMANAGER_POOL_2_ALLOCATION_SIZE, MEMORYMANAGER_POOL_2_ALLOCATION_COUNT)];
static uint8_t pool3[Pool_CalculateSize(MEMORYMANAGER_POOL_3_ALLOCATION_SIZE, MEMORYMANAGER_POOL_3_ALLOCATION_COUNT)];
static uint8_t pool4[Pool_CalculateSize(MEMORYMANAGER_POOL_4_ALLOCATION_SIZE, MEMORYMANAGER_POOL_4_ALLOCATION_COUNT)];
static const uint8_t* const pool1End = pool1 + sizeof(pool1) - 1;
static const uint8_t* const pool2End = pool2 + sizeof(pool2) - 1;
static const uint8_t* const pool3End = pool3 + sizeof(pool3) - 1;
static const uint8_t* const pool4End = pool4 + sizeof(pool4) - 1;

void MemoryManager_Initialize()
{
	Pool_Initialize(pool1, MEMORYMANAGER_POOL_1_ALLOCATION_SIZE, MEMORYMANAGER_POOL_1_ALLOCATION_COUNT);
	Pool_Initialize(pool2, MEMORYMANAGER_POOL_2_ALLOCATION_SIZE, MEMORYMANAGER_POOL_2_ALLOCATION_COUNT);
	Pool_Initialize(pool3, MEMORYMANAGER_POOL_3_ALLOCATION_SIZE, MEMORYMANAGER_POOL_3_ALLOCATION_COUNT);
	Pool_Initialize(pool4, MEMORYMANAGER_POOL_4_ALLOCATION_SIZE, MEMORYMANAGER_POOL_4_ALLOCATION_COUNT);
}

void* MemoryManager_Allocate(uint8_t size)
{
	if (size <= MEMORYMANAGER_POOL_1_ALLOCATION_SIZE)
	{
		void* block = Pool_AllocateBlock(pool1);
		if (block != NULL)
		{
			Diagnostics_SendEvent(DIAGNOSTICS_POOL_1_ALLOCATION);
			return block;
		}
		Diagnostics_SendEvent(DIAGNOSTICS_POOL_1_EXHAUSTED);
	}

	if (size <= MEMORYMANAGER_POOL_2_ALLOCATION_SIZE)
	{
		void* block = Pool_AllocateBlock(pool2);
		if (block != NULL)
		{
			Diagnostics_SendEvent(DIAGNOSTICS_POOL_2_ALLOCATION);
			return block;
		}
		Diagnostics_SendEvent(DIAGNOSTICS_POOL_2_EXHAUSTED);
	}

	if (size <= MEMORYMANAGER_POOL_3_ALLOCATION_SIZE)
	{
		void* block = Pool_AllocateBlock(pool3);
		if (block != NULL)
		{
			Diagnostics_SendEvent(DIAGNOSTICS_POOL_3_ALLOCATION);
			return block;
		}
		Diagnostics_SendEvent(DIAGNOSTICS_POOL_3_EXHAUSTED);
	}

	if (size <= MEMORYMANAGER_POOL_4_ALLOCATION_SIZE)
	{
		void* block = Pool_AllocateBlock(pool4);
		if (block != NULL)
		{
			Diagnostics_SendEvent(DIAGNOSTICS_POOL_4_ALLOCATION);
			return block;
		}
		Diagnostics_SendEvent(DIAGNOSTICS_POOL_4_EXHAUSTED);
	}

	Diagnostics_SendEvent(DIAGNOSTICS_MEMORY_ALLOCATION_FAILED);

	return NULL;
}

void MemoryManager_Release(void* block)
{
	uint8_t* b = block;

	if (pool1 <= b && b <= pool1End)
	{
		if (Pool_ReleaseBlock(pool1, block))
		{
			Diagnostics_SendEvent(DIAGNOSTICS_POOL_1_RELEASE);
			return;
		}
	}
	else if (pool2 <= b && b <= pool2End)
	{
		if (Pool_ReleaseBlock(pool2, block))
		{
			Diagnostics_SendEvent(DIAGNOSTICS_POOL_2_RELEASE);
			return;
		}
	}
	else if (pool3 <= b && b <= pool3End)
	{
		if (Pool_ReleaseBlock(pool3, block))
		{
			Diagnostics_SendEvent(DIAGNOSTICS_POOL_3_RELEASE);
			return;
		}
	}
	else if (pool4 <= b && b <= pool4End)
	{
		if (Pool_ReleaseBlock(pool4, block))
		{
			Diagnostics_SendEvent(DIAGNOSTICS_POOL_4_RELEASE);
			return;
		}
	}

	Diagnostics_SendEvent(DIAGNOSTICS_MEMORY_RELEASE_FAILED);
}
