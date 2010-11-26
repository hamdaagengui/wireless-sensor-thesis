/*
 * AutomaticMemoryManager.h
 *
 *  Created on: 25/11/2010
 *      Author: coma
 */

#ifndef AUTOMATICMEMORYMANAGER_H_
#define AUTOMATICMEMORYMANAGER_H_

#include "Globals.h"

typedef struct
{
	void* obj;
} ref;

ref r;

#define DEREF(reference, type) (*((type*)reference.obj))

extern bool AMM_Allocate(ref* reference, uint8_t size);
extern bool AMM_CreateReference(ref* reference);
extern bool AMM_Allocate(ref* reference, uint8_t size);

#endif /* AUTOMATICMEMORYMANAGER_H_ */
