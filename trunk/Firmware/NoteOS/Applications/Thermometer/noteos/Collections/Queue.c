/*
 * Queue.c
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#include "Queue.h"

void Queue_Initialize(void* q, uint8_t elementSize, uint8_t elementCount)
{
	queue* _q = q;

	_q->head = 0;
	_q->tail = 0;
	_q->free = elementCount;
	_q->size = elementCount;
	_q->elementSize = elementSize;
}

bool Queue_IsFull(void* q)
{
	queue* _q = q;

	return _q->free == 0;
}

bool Queue_IsEmpty(void* q)
{
	queue* _q = (queue*) q;

	return _q->free == _q->size;
}

void* Queue_Head(void* q)
{
	queue* _q = (queue*) q;
	uint16_t position = _q->head;
	position *= _q->elementSize;

	return (void*) &_q->buffer[position];
}

void* Queue_Tail(void* q)
{
	queue* _q = (queue*) q;
	uint16_t position = _q->tail;
	position *= _q->elementSize;

	return (void*) &_q->buffer[position];
}

void Queue_AdvanceHead(void* q)
{
	queue* _q = (queue*) q;

	if (++_q->head >= _q->size)
	{
		_q->head = 0;
	}

	atomic(_q->free--);
}

void Queue_AdvanceTail(void* q)
{
	queue* _q = (queue*) q;

	if (++_q->tail >= _q->size)
	{
		_q->tail = 0;
	}

	atomic(_q->free++);
}
