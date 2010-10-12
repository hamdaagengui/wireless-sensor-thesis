/*
 * Pool.h
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#ifndef POOL_H_
#define POOL_H_

#include "../Globals.h"

typedef struct
{
	uint8_t blockSize;
	uint8_t blockCount;
	uint8_t next;
	uint8_t blocksFree;
	uint8_t buffer[];
} pool;

typedef struct
{
	bool used;
	uint8_t data[];
} pool_block;

// Usage example: uint8_t poo[Pool_CalculateSize(8, 10)];
#define Pool_CalculateSize(blockSize, blockCount) (sizeof(pool) + ((sizeof(pool_block) + (blockSize)) * (blockCount)))

extern void Pool_Initialize(void* p, uint8_t blockSize, uint8_t blockCount);
extern bool Pool_IsExhausted(void* p);
extern void* Pool_AllocateBlock(void* p);
extern void Pool_ReleaseBlock(void* p, void* block);

#endif /* POOL_H_ */
