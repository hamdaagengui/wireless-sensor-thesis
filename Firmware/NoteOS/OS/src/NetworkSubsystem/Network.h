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
	PROPERTY_STATUS_INVALID_SENSOR,
	PROPERTY_STATUS_INVALID_PROPERTY,
	PROPERTY_STATUS_READ_ONLY,
	PROPERTY_STATUS_WRITE_ONLY,
	PROPERTY_STATUS_UNDERFLOW,
	PROPERTY_STATUS_OVERFLOW
} property_status;

//
//
// Layer headers
typedef struct
{
	uint8_t destination :4;
	uint8_t source :4;
	uint8_t type :5;
	uint8_t :3;
} link_header;

typedef struct
{
	uint8_t receiver :4;
	uint8_t sender :4;
} network_header;

typedef struct
{
} bed_header;

typedef struct
{
	uint8_t sequenceNumber;
} rdd_header;

//
//
// Combined layer headers
typedef struct
{
	link_header link;
	network_header network;
} link_network_header;

typedef struct
{
	link_header link;
	network_header network;
	bed_header transport;
} link_network_bed_header;

typedef struct
{
	link_header link;
	network_header network;
	rdd_header transport;
} link_network_rdd_header;

//
//
// Link layer packets
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

// Network layer packets
typedef struct
{
	link_header link;
	network_header network;
	uint8_t distances[15];
} network_routes_packet;

// Transport layer packets


// Application layer packets

typedef struct
{
	link_header link;
	network_header network;
	bed_header transport;
	uint8_t data[];
} application_raw_bed_packet;

typedef struct
{
	link_header link;
	network_header network;
	bed_header transport;
	uint8_t serialNumber[16];
} application_join_request_packet;

typedef struct
{
	link_header link;
	network_header network;
	bed_header transport;
	uint8_t serialNumber[16];
	uint8_t assignedAddress;
} application_join_response_packet;

typedef struct
{
	link_header link;
	network_header network;
	rdd_header transport;
} transport_rdd_acknowledge_packet;

typedef struct
{
	link_header link;
	network_header network;
	rdd_header transport;
	uint8_t sensor;
	uint8_t data[];
} application_sensor_data_packet;

typedef struct
{
	link_header link;
	network_header network;
	rdd_header transport;
	uint8_t sensor;
	uint8_t property;
	uint8_t data[];
} application_set_request_packet;

typedef struct
{
	link_header link;
	network_header network;
	rdd_header transport;
	uint8_t status;
} application_set_response_packet;

typedef struct
{
	link_header link;
	network_header network;
	rdd_header transport;
	uint8_t sensor;
	uint8_t property;
} application_get_request_packet;

typedef struct
{
	link_header link;
	network_header network;
	rdd_header transport;
	uint8_t status;
	uint8_t data[];
} application_get_response_packet;

//typedef struct
//{
//	link_header link;
//	network_header network;
//	char fieldName[10];
//	uint8_t data[];
//} write_packet;
//
//typedef struct
//{
//	link_header link;
//	network_header network;
//	uint8_t status;
//} write_complete_packet;
//
//typedef struct
//{
//	link_header link;
//	network_header network;
//	char fieldName[10];
//} read_packet;
//
//typedef struct
//{
//	link_header link;
//	network_header network;
//	uint8_t status;
//	uint8_t data[];
//} read_complete_packet;

extern void Network_Initialize();
extern void Network_Start();

extern void Network_SetRawBedDataHandler(block_handler handler);

#include "BedProtocol.h"

//extern void* Network_CreateBedPacket(uint8_t receiver, uint8_t type, uint8_t size);
//extern bool Network_QueueBedPacket();

#include "RddProtocol.h"

//extern void* Network_CreateSensorDataPacket(uint8_t receiver, uint8_t sensor, uint8_t dataSize);
//extern void Network_CreateSetResponsePacket(uint8_t receiver, property_status status);
//extern void* Network_CreateGetResponsePacket(uint8_t receiver, property_status status, uint8_t dataSize);
//extern void Network_QueueRddPacket();

#include "ApplicationProtocols.h"

extern bool Network_SetProperty(uint8_t node, uint8_t sensor, uint8_t property, void* data, uint8_t length, result_handler resultHandler);
extern bool Network_GetProperty(uint8_t node, uint8_t sensor, uint8_t property, void* data, uint8_t length, result_handler resultHandler);

//// Interface to non-volatile node user information
//extern bool Network_Load(char* name, void* data, uint8_t length, result_handler resultHandler);
//extern bool Network_Store(char* name, void* data, uint8_t length, result_handler resultHandler);

//// Interface to raw data memory access
//extern bool Network_Read(uint16_t address, void* data, uint8_t length, result_handler resultHandler);
//extern bool Network_Write(uint16_t address, void* data, uint8_t length, result_handler resultHandler);

#endif /* NETWORK_H_ */
