/*
 * HeartRateVariability.c
 *
 *  Created on: 21/10/2010
 *      Author: coma
 */

#include "HeartRateVariability.h"

enum
{
	PROPERTY_CURRENT_VALUE,
	PROPERTY_SAMPLE_INTERVAL,
	PROPERTY_BIN_SIZE
};

static bool Initialize(uint8_t id);
static void Set(application_set_request_packet* packet);
static void Get(application_get_request_packet* packet);

const sensor_interface heartRateVariabilityInterface = { Initialize, Set, Get };
static uint8_t assignedId;

bool Initialize(uint8_t id)
{
	assignedId = id;

	return true;
}

static void Set(application_set_request_packet* packet)
{
	switch (packet->property)
	{
		case PROPERTY_SAMPLE_INTERVAL:
			break;

		case PROPERTY_BIN_SIZE:
			break;

		default:
			Network_CreateSetResponsePacket(packet->network.sender, PROPERTY_STATUS_INVALID_PROPERTY);
			Network_SendRddPacket();
			break;
	}
}

static void Get(application_get_request_packet* packet)
{
	switch (packet->property)
	{
		case PROPERTY_CURRENT_VALUE:
			break;

		case PROPERTY_SAMPLE_INTERVAL:
			break;

		case PROPERTY_BIN_SIZE:
			break;

		default:
			Network_CreateGetResponsePacket(packet->network.sender, PROPERTY_STATUS_INVALID_PROPERTY, 0);
			Network_SendRddPacket();
			break;
	}
}
