/*
 * Network.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../Globals.h"

typedef enum
{
	PROPERTY_STATUS_SUCCESS,
	PROPERTY_STATUS_UNKNOWN_PROPERTY,
	PROPERTY_STATUS_UNDERFLOW,
	PROPERTY_STATUS_OVERFLOW
} property_status;

typedef struct
{
	uint8_t destination :4;
	uint8_t source :4;
	uint8_t type :4;
	uint8_t :4;
} link_header;

typedef struct
{
	uint8_t receiver :4;
	uint8_t sender :4;
} network_header;

typedef struct
{
	uint8_t sequenceNumber;
} transport_header;

// combined headers
typedef struct
{
	link_header link;
	network_header network;
} link_network_header;

typedef struct
{
	link_header link;
	network_header network;
	transport_header transport;
	uint8_t payload[];
} link_network_transport_header;

typedef struct
{
	link_header link;
	uint8_t slot;
	int8_t transmissionPowerLevel;
} link_rts_packet;

typedef struct
{
	link_header link;
} link_cts_packet;

typedef struct
{
	link_header link;
} link_acknowledge_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t distances[15];
} network_routes_packet;

typedef struct
{
	link_header link;
	network_header network;
	transport_header transport;
} transport_acknowledge_packet;

typedef struct
{
	link_header link;
	network_header network;
	transport_header transport;
	uint8_t sensor;
	uint8_t data[];
} application_sensor_data_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t sensor;
	uint8_t property;
	uint8_t data[];
} set_request_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
} set_response_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t sensor;
	uint8_t property;
} get_request_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
	uint8_t data[];
} get_response_packet;

typedef struct
{
	link_header link;
	network_header network;
	char fieldName[10];
	uint8_t data[];
} write_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
} write_complete_packet;

typedef struct
{
	link_header link;
	network_header network;
	char fieldName[10];
} read_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
	uint8_t data[];
} read_complete_packet;

extern void Network_Initialize();
extern void Network_SetAddress(uint8_t id);
extern void Network_Handlers(block_handler sensorDataHandler);

extern void* Network_CreateTransportPacket(uint8_t receiver, uint8_t type, uint8_t size);
extern void* Network_CreateSensorDataPacket(uint8_t receiver, uint8_t sensor, uint8_t dataSize);
extern void* Network_CreateGetResponsePacket(uint8_t receiver, property_status status, uint8_t dataSize);
extern void Network_SendPacket();

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
