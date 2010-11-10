/*
 * Network.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../Globals.h"

extern void Network_Initialize(bool master);
extern void Network_SetId(uint8_t id);
extern void Network_SetDataHandler(block_handler handler);
extern bool Network_SendData(uint8_t receiver, void* data, uint8_t length);

extern void Network_TimerEvent();

// Interface to sensor properties
extern bool Network_SetProperty(uint8_t node, uint8_t sensor, uint8_t property, void* data, uint8_t length, result_handler resultHandler);
extern bool Network_GetProperty(uint8_t node, uint8_t sensor, uint8_t property, void* data, uint8_t length, result_handler resultHandler);

// Interface to non-volatile node user information
extern bool Network_Load(char* name, void* data, uint8_t length, result_handler resultHandler);
extern bool Network_Store(char* name, void* data, uint8_t length, result_handler resultHandler);

// Interface to raw data memory access
extern bool Network_Read(uint16_t address, void* data, uint8_t length, result_handler resultHandler);
extern bool Network_Write(uint16_t address, void* data, uint8_t length, result_handler resultHandler);

#endif /* NETWORK_H_ */
