/*
 * MemoryManager.h
 *
 *  Created on: 25/10/2010
 *      Author: coma
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

extern void MemoryManager_Initialize();
extern void* MemoryManager_AllocateNetworkBlock();
extern void MemoryManager_ReleaseNetworkBlock(void* block);
extern void* MemoryManager_AllocateSensorBlock();
extern void MemoryManager_ReleaseSensorBlock(void* block);
extern void MemoryManager_ReleaseAnyBlock(void* block);

#endif /* MEMORYMANAGER_H_ */
