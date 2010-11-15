/*
 * HeartRateVariability.c
 *
 *  Created on: 21/10/2010
 *      Author: coma
 */

#include "HeartRateVariability.h"

enum
{
	PROPERTY_SAMPLE_RATE,
	PROPERTY_BIN_SIZE
};

static void Initialize();
static void Reset();
static bool Set(uint8_t propertyId, void* value);
static bool Get(uint8_t propertyId, void* value);

void HeartRateVariability_Create(sensor_interface* interface)
{
	interface->initialize = Initialize;
	interface->reset = Reset;
	interface->set = Set;
	interface->get = Get;
}

static void Initialize()
{

}

static void Reset()
{

}

static bool Set(uint8_t propertyId, void* value)
{
	switch (propertyId)
	{
		case PROPERTY_SAMPLE_RATE:
			break;

		case PROPERTY_BIN_SIZE:
			break;

		default:
			return false;
	}

	return true;
}

static bool Get(uint8_t propertyId, void* value)
{
	switch (propertyId)
	{
		case PROPERTY_SAMPLE_RATE:
			break;

		case PROPERTY_BIN_SIZE:
			break;

		default:
			return false;
	}

	return true;
}
