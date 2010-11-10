/*
 * Pool.c
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#include "Pool.h"

void Pool_Initialize(void* p, uint8_t blockSize, uint8_t blockCount)
{
	pool* _p = p;

	_p->blockSize = blockSize;
	_p->blockCount = blockCount;
	_p->next = 0;
	_p->blocksFree = blockCount;

	uint8_t size = sizeof(pool_block) + blockSize;
	void* buf = &_p->buffer;


	// TODO This can be omitted if it can be guaranteed that the pool data is declared static or is global (C99 requires that these be set to null by the runtime)
	for (uint8_t i = 0; i < blockCount; i++)
	{
		((pool_block*) buf)->used = false;
		buf += size;
	}
}

bool Pool_IsExhausted(void* p)
{
	pool* _p = p;

	return _p->blocksFree == 0;
}

// TODO Use 'next' to increase allocation speed.
void* Pool_AllocateBlock(void* p)
{
	pool* _p = p;

	if (_p->blocksFree == 0)
	{
		// error
		return NULL;
	}

	uint8_t size = sizeof(pool_block) + _p->blockSize;
	void* buf = &_p->buffer;

	for (uint8_t i = 0; i < _p->blockCount; i++)
	{
		pool_block* pb = buf;
		if (pb->used == false)
		{
			pb->used = true;
			return &pb->data;
		}
		buf += size;
	}

	return NULL;
}

// TODO Maybe use 'next' to reverse to specified block faster
bool Pool_ReleaseBlock(void* p, void* block)
{
	pool* _p = p;

	uint8_t size = sizeof(pool_block) + _p->blockSize;
	void* buf = &_p->buffer;

	for (uint8_t i = 0; i < _p->blockCount; i++)
	{
		pool_block* pb = buf;
		if (&pb->data == block)
		{
			pb->used = false;
			return true;
		}
		buf += size;
	}

	return false;
}
