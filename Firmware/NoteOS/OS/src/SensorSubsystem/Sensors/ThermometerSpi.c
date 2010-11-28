/*
 * ThermometerSpi.c
 *
 *  Created on: 28/11/2010
 *      Author: coma
 */

#include "ThermometerSpi.h"
#include "../../EventSubsystem/Timer.h"
#include "../../Devices/MAX6662.h"
#include "../../PlatformAbstractionLayer/PlatformAbstractionLayer.h"
#include "../../NetworkSubsystem/Network.h"

enum
{
	PROPERTY_CURRENT_VALUE,
	PROPERTY_SAMPLE_INTERVAL,
	PROPERTY_UPPER_THRESHOLD,
	PROPERTY_LOWER_THRESHOLD
};

static bool Initialize(uint8_t id);
static bool Set(set_request_packet* packet);
static bool Get(get_request_packet* packet);
static void TakeSample();
static void SampleTaken();

static const sensor_interface interface = { Initialize, Set, Get };
static uint8_t assignedId;
static timer_configuration timer;
static uint32_t interval = 2000000;
static int16_t temperature;

const sensor_interface* ThermomoterSpi_GetInterface()
{
	return &interface;
}

static bool Initialize(uint8_t id)
{
	assignedId = id;

	Timer_CreateConfiguration(&timer, interval, TIMER_MODE_RELAXED_CONTINUES, TakeSample);
	Timer_Start(&timer);

	return true;
}

static bool Set(set_request_packet* packet)
{
	switch (packet->property)
	{
		case PROPERTY_SAMPLE_INTERVAL:
			uint32_t* value = Network_CreateGetResponsePacket(0, PROPERTY_STATUS_SUCCESS, sizeof(uint32_t));
			*value = interval;
			Network_SendPacket();
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
			{
				uint32_t* value = Network_CreateGetResponsePacket(0, PROPERTY_STATUS_SUCCESS, sizeof(uint32_t));
				*value = interval;
				Network_SendPacket();
			}
			break;

		default:
			return false;
	}

	return true;
}

static void TakeSample()
{
	MAX6662_GetTemperature(&max6662Configuration, &temperature, SampleTaken);
}

static void SampleTaken()
{
	//EventDispatcher_Publish()
}
