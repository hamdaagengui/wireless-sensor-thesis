/*
 * FIFO.c
 *
 *  Created on: 03/10/2010
 *      Author: coma
 */

#include "FIFO.h"

void FIFO_Initialize(void* f, uint8_t byteCount)
{
	fifo* _f = f;

	_f->head = 0;
	_f->tail = 0;
	_f->free = byteCount;
	_f->size = byteCount;
}

bool FIFO_IsEmpty(void* f)
{
	fifo* _f = f;

	return _f->free == _f->size;
}

uint8_t FIFO_PeekFirst(void* f)
{
	fifo* _f = f;

	return _f->buffer[_f->head];
}

bool FIFO_WriteByte(void* f, uint8_t value)
{
	fifo* _f = f;

	if (_f->free < 1)
	{
		return false;
	}

	_f->buffer[_f->head] = value;

	if (++_f->head >= _f->size)
	{
		_f->head = 0;
	}

	_f->free--;

	return true;
}

bool FIFO_Write(void* f, void* buffer, uint8_t length)
{
	fifo* _f = f;
	uint8_t* _b = buffer;

	if (_f->free < length)
	{
		return false;
	}

	while (length--)
	{
		_f->buffer[_f->head] = *_b++;

		if (++_f->head >= _f->size)
		{
			_f->head = 0;
		}
	}

	_f->free -= length;

	return true;
}

bool FIFO_Read(void* f, void* buffer, uint8_t length)
{
	fifo* _f = f;
	uint8_t* _b = buffer;

	if ((_f->size - _f->free) < length)
	{
		return false;
	}

	while (length--)
	{
		*_b++ = _f->buffer[_f->tail];

		if (++_f->tail >= _f->size)
		{
			_f->tail = 0;
		}
	}

	_f->free += length;

	return true;
}
