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
static bool Set(set_request_packet* packet);
static bool Get(get_request_packet* packet);

static const sensor_interface interface = { Initialize, Set, Get };
static uint8_t assignedId;

const sensor_interface* HeartRateVariability_GetInterface()
{
	return &interface;
}

bool Initialize(uint8_t id)
{
	assignedId = id;

	return true;
}

static bool Set(set_request_packet* packet)
{
	switch (packet->property)
	{
		case PROPERTY_SAMPLE_INTERVAL:
			break;

		case PROPERTY_BIN_SIZE:
			break;

		default:
			return false;
	}

	return true;
}

static bool Get(get_request_packet* packet)
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
			return false;
	}

	return true;
}
