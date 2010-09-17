/*
 * Queue.c
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#include "Queue.h"
#include <string.h>

void Queue_Initialize(void* q, uint8_t elementSize, uint8_t elementCount)
{
	queue* _q = q;

	_q->in = 0;
	_q->out = 0;
	_q->elementSize = elementSize;
	_q->elementsFree = elementCount;
	_q->elementsMax = elementCount;

}

void Queue_Put(void* q, void* data)
{
	queue* _q = q;

	if (_q->elementsFree > 0)
	{
		void* des = &_q->buffer[_q->in * _q->elementSize];
		memcpy(des, data, _q->elementSize);
		if (++_q->in >= _q->elementsMax)
		{
			_q->in = 0;
		}
		atomic(_q->elementsFree--);
	}
}

void Queue_Get(void* q, void* data)
{
	queue* _q = q;

	if (_q->elementsFree < _q->elementsMax)
	{
		void* src = &_q->buffer[_q->out * _q->elementSize];
		memcpy(data, src, _q->elementSize);
		if (++_q->out >= _q->elementsMax)
		{
			_q->out = 0;
		}
		atomic(_q->elementsFree++);
	}
}

bool Queue_IsFull(void* q)
{
	queue* _q = q;

	return _q->elementsFree == 0;
}

bool Queue_IsEmpty(void* q)
{
	queue* _q = (queue*) q;

	return _q->elementsFree == _q->elementsMax;
}

//void* Queue_Head(void* q)
//{
//	queue* _q = (queue*) q;
//}
//
//void* Queue_Tail(void* q)
//{
//
//}
//
//void AdvanceHead(void* q)
//{
//
//}
//
//void AdvanceTail(void* q)
//{
//
//}
