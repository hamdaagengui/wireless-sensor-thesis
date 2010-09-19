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

	uint8_t size = sizeof(poolBlock) + blockSize;
	void* buf = &_p->buffer;

	for (uint8_t i = 0; i < blockCount; i++)
	{
		((poolBlock*) buf)->used = false;
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

	uint8_t blockCount = _p->blockCount;
	uint8_t size = sizeof(poolBlock) + _p->blockSize;
	void* buf = &_p->buffer;

	for (uint8_t i = 0; i < blockCount; i++)
	{
		poolBlock* pb = buf;
		if (pb->used == false)
		{
			pb->used = true;
			return &pb->data;
		}
		buf += size;
	}

	return NULL;
}

void Pool_ReleaseBlock(void* p, void* block)
{
	// TODO Implement
	// TODO Maybe use 'next' to reverse to specified block faster
}

//
//void Pool_Initialize(void* q, uint8_t elementSize, uint8_t elementCount)
//{
//	queue* _q = q;
//
//	_q->head = 0;
//	_q->tail = 0;
//	_q->free = elementCount;
//	_q->size = elementCount;
//	_q->elementSize = elementSize;
//}
//
//bool Pool_IsFull(void* q)
//{
//	queue* _q = q;
//
//	return _q->free == 0;
//}
//
//bool Pool_IsEmpty(void* q)
//{
//	queue* _q = (queue*) q;
//
//	return _q->free == _q->size;
//}
//
//void* Pool_Head(void* q)
//{
//	queue* _q = (queue*) q;
//	uint16_t position = _q->head;
//	position *= _q->elementSize;
//
//	return (void*) &_q->buffer[position];
//}
//
//void* Pool_Tail(void* q)
//{
//	queue* _q = (queue*) q;
//	uint16_t position = _q->tail;
//	position *= _q->elementSize;
//
//	return (void*) &_q->buffer[position];
//}
//
//void Pool_AdvanceHead(void* q)
//{
//	queue* _q = (queue*) q;
//
//	if (++_q->head >= _q->size)
//	{
//		_q->head = 0;
//	}
//
//	atomic(_q->free--);
//}
//
//void Pool_AdvanceTail(void* q)
//{
//	queue* _q = (queue*) q;
//
//	if (++_q->tail >= _q->size)
//	{
//		_q->tail = 0;
//	}
//
//	atomic(_q->free++);
//}
