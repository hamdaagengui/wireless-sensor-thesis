/*
 * TemperatureSensorMax6662.c
 *
 *  Created on: 28/11/2010
 *      Author: coma
 */

#include "TemperatureSensorMax6662.h"
#include "../../Devices/MAX6662.h"
#include "../../EventSubsystem/Timer.h"
#include "../../BoardSupportPackage/BoardSupportPackage.h"
#include "../../MemorySubsystem/MemoryManager.h"

enum
{
	PROPERTY_CURRENT_VALUE,
	PROPERTY_SAMPLE_INTERVAL
};

static bool Initialize(uint8_t id);
static void Set(application_set_request_packet* packet);
static void Get(application_get_request_packet* packet);
static void TakeSample();
static void SampleTaken();

const sensor_interface temperatureSensorMax6662Interface = { Initialize, Set, Get };
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
	uint8_t sender = packet->network.sender;

	switch (packet->property)
	{
		case PROPERTY_CURRENT_VALUE:
			Network_CreateSetResponsePacket(sender, PROPERTY_STATUS_READ_ONLY);
			Network_QueueRddPacket();
			break;

		case PROPERTY_SAMPLE_INTERVAL:
			*(uint32_t*) Network_CreateGetResponsePacket(sender, PROPERTY_STATUS_SUCCESS, sizeof(uint32_t)) = interval;
			Network_QueueRddPacket();
			break;

		default:
			Network_CreateSetResponsePacket(sender, PROPERTY_STATUS_INVALID_PROPERTY);
			Network_QueueRddPacket();
			break;
	}
}

static void Get(application_get_request_packet* packet)
{
	uint8_t sender = packet->network.sender;

	switch (packet->property)
	{
		case PROPERTY_CURRENT_VALUE:
			*(uint16_t*) Network_CreateGetResponsePacket(sender, PROPERTY_STATUS_SUCCESS, sizeof(uint16_t)) = temperature;
			Network_QueueRddPacket();
			break;

		case PROPERTY_SAMPLE_INTERVAL:
			*(uint32_t*) Network_CreateGetResponsePacket(sender, PROPERTY_STATUS_SUCCESS, sizeof(uint32_t)) = interval;
			Network_QueueRddPacket();
			break;

		default:
			Network_CreateGetResponsePacket(sender, PROPERTY_STATUS_INVALID_PROPERTY, 0);
			Network_QueueRddPacket();
			break;
	}
}

static void TakeSample()
{
	MAX6662_GetTemperature(&max6662Configuration, &temperature, SampleTaken);
}

static void SampleTaken()
{
	sensor_event* se = MemoryManager_Allocate(sizeof(sensor_event) + sizeof(int16_t));
	se->sensor = SENSOR_TEMPERATURE_EAR_LEFT;
	*(int16_t*) &se->data = temperature;
	EventDispatcher_Publish(EVENT_SENSOR_DATA, se);
}
