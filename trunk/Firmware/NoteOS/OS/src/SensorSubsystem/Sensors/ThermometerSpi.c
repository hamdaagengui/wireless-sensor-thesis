/*
 * ThermometerSpi.c
 *
 *  Created on: 28/11/2010
 *      Author: coma
 */

#include "ThermometerSpi.h"
#include "../../Devices/MAX6662.h"
#include "../../EventSubsystem/Timer.h"
#include "../../BoardSupportPackage/BoardSupportPackage.h"

enum
{
	PROPERTY_CURRENT_VALUE,
	PROPERTY_SAMPLE_INTERVAL,
	PROPERTY_UPPER_THRESHOLD,
	PROPERTY_LOWER_THRESHOLD
};

static bool Initialize(uint8_t id);
static void Set(application_set_request_packet* packet);
static void Get(application_get_request_packet* packet);
static void TakeSample();
static void SampleTaken();

const sensor_interface thermomoterSpiInterface = { Initialize, Set, Get };
static uint8_t assignedId;

static timer_configuration timer;
static uint32_t interval = 2000000;
static int16_t temperature;

static bool Initialize(uint8_t id)
{
	assignedId = id;

	Timer_CreateConfiguration(&timer, interval, TIMER_MODE_RELAXED_CONTINUES, TakeSample);
	Timer_Start(&timer);

	return true;
}

static void Set(application_set_request_packet* packet)
{
	switch (packet->property)
	{
		case PROPERTY_SAMPLE_INTERVAL:
			{
				uint32_t* value = Network_CreateGetResponsePacket(0, PROPERTY_STATUS_SUCCESS, sizeof(uint32_t));
				*value = interval;
				Network_SendPacket();
			}
			break;

		default:
			Network_CreateSetResponsePacket(packet->network.sender, PROPERTY_STATUS_INVALID_PROPERTY);
			Network_SendPacket();
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
			{
				uint32_t* value = Network_CreateGetResponsePacket(0, PROPERTY_STATUS_SUCCESS, sizeof(uint32_t));
				*value = interval;
				Network_SendPacket();
			}
			break;

		default:
			Network_CreateGetResponsePacket(packet->network.sender, PROPERTY_STATUS_INVALID_PROPERTY, 0);
			Network_SendPacket();
			break;
	}
}

static void TakeSample()
{
	MAX6662_GetTemperature(&max6662Configuration, &temperature, SampleTaken);
}

static void SampleTaken()
{
	//EventDispatcher_Publish()
}
