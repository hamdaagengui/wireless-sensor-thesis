/*
 * Internal.c
 *
 *  Created on: 29/11/2010
 *      Author: coma
 */

#include "Internal.h"

enum
{
	PROPERTY_ZERO,
	PROPERTY_ENERGY_LEVEL
};

static bool Initialize(uint8_t id);
static void Set(application_set_request_packet* packet);
static void Get(application_get_request_packet* packet);

const sensor_interface internalInterface = { Initialize, Set, Get };

static uint8_t assignedId;

static bool Initialize(uint8_t id)
{
	assignedId = id;

	return true;
}

static void Set(application_set_request_packet* packet)
{
	switch (packet->property)
	{
		case PROPERTY_ZERO:
			ApplicationProtocols_SendSetResponse(packet->network.sender, PROPERTY_STATUS_READ_ONLY);
			break;

		default:
			ApplicationProtocols_SendSetResponse(packet->network.sender, PROPERTY_STATUS_INVALID_PROPERTY);
			break;
	}
}

static void Get(application_get_request_packet* packet)
{
	switch (packet->property)
	{
		case PROPERTY_ZERO:
			{
				uint8_t zero = 0;
				ApplicationProtocols_SendGetResponse(packet->network.sender, PROPERTY_STATUS_SUCCESS, &zero, sizeof(uint8_t));
			}
			break;

		default:
			ApplicationProtocols_SendGetResponse(packet->network.sender, PROPERTY_STATUS_INVALID_PROPERTY, NULL, 0);
			break;
	}
}
