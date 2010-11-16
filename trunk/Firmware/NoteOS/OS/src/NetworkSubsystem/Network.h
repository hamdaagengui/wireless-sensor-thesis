/*
 * Network.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../Globals.h"

// Stack management
extern void Network_Initialize();
extern void Network_SetAddress(uint8_t id);
extern void Network_Handlers(block_handler sensorDataHandler);

extern void Network_SendPacket();

// Transport layer services
extern void* Network_CreateTransportPacket(uint8_t receiver, uint8_t type, uint8_t size);

// Application layer services
extern void* Network_CreateSensorDataPacket(uint8_t receiver, uint8_t sensor, uint8_t dataSize);
extern bool Network_SetProperty(uint8_t node, uint8_t sensor, uint8_t property, void* data, uint8_t length, result_handler resultHandler);
extern bool Network_GetProperty(uint8_t node, uint8_t sensor, uint8_t property, void* data, uint8_t length, result_handler resultHandler);

//// Interface to non-volatile node user information
//extern bool Network_Load(char* name, void* data, uint8_t length, result_handler resultHandler);
//extern bool Network_Store(char* name, void* data, uint8_t length, result_handler resultHandler);

//// Interface to raw data memory access
//extern bool Network_Read(uint16_t address, void* data, uint8_t length, result_handler resultHandler);
//extern bool Network_Write(uint16_t address, void* data, uint8_t length, result_handler resultHandler);

#endif /* NETWORK_H_ */

/*
 void SensorSendExample()
 {
 uint16_t* value = Network_CreateSensorDataPacket(0, 123, sizeof(uint16_t));

 *value = 1234;

 Network_SendPacket();
 }
 */
