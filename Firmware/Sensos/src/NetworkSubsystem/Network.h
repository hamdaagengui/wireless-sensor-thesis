/*
 * Network.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"

enum
{
	FLOW_SPONTANEOUS,
	FLOW_CONSTANT,
	FLOW_BURSTY
};

enum
{
	CLASSIFICATION_BULK,
	CLASSIFICATION_STREAM,
	CLASSIFICATION_NOTIFICATION
};

typedef struct
{
	uint8_t flowType;
	uint32_t bandwidth; // in units of bytes per second
	completion_handler completed;
} network_configuration;

typedef struct
{
	uint8_t id;
	uint32_t bandwidth;
	uint8_t flow;
	uint8_t maxLoss;
} sensor;

extern void Network_Initialize();
extern void Network_Subscribe(network_configuration* configuration);
extern void Network_Start();

extern void Network_ConfigureNode(uint8_t serialNumber[], uint8_t sensorCount, uint8_t slotLength);
extern void Network_SendSensorData(uint8_t sensorId, void* data, uint8_t length);

#endif /* NETWORK_H_ */
