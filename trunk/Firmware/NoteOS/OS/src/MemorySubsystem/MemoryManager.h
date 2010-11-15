/*
 * MemoryManager.h
 *
 *  Created on: 25/10/2010
 *      Author: coma
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

extern void MemoryManager_Initialize();
extern void* MemoryManager_Allocate(uint8_t size);
extern void MemoryManager_Release(void* block);

#endif /* MEMORYMANAGER_H_ */
