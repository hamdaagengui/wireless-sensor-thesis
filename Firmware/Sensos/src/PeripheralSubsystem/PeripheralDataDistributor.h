/*
 * PeripheralDataDistributor.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef PERIPHERALDATADISTRIBUTOR_H_
#define PERIPHERALDATADISTRIBUTOR_H_

#include <Globals.h>

typedef void (*peripheralEventHandler)(void* data);

// Initialization
void PeripheralDataDistributor_Initialize();
void PeripheralDataDistributor_Start();
void PeripheralDataDistributor_ProcessEvents();

// Subscribers
void PeripheralDataDistributor_RegisterSubscriber(uint8_t event, peripheralEventHandler handler);

// Publishers
void* PeripheralDataDistributor_GetBuffer();
void* PeripheralDataDistributor_RegisterPublisher(uint8_t id);
void* PeripheralDataDistributor_Publish(uint8_t peripheral, void* data);

#endif /* PERIPHERALDATADISTRIBUTOR_H_ */
