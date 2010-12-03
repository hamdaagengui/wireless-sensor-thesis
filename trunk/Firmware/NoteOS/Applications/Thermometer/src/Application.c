/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "NoteOS.h"

void ThermoUpdate(sensor_id sensor, void* data)
{
	int16_t* v = Network_CreateSensorDataPacket(0, 1, 2);
	*v = *((int16_t*) data);
	Network_QueueRddPacket();
}

void Ping()
{
	transport_bed_header* p = BedProtocol_CreatePacket(1, 15, sizeof(transport_bed_header));
	Network_QueueBedPacket();

	Leds_RedToggle();
}

timer_configuration tim;

void Synced(event e, void* data)
{
	Leds_YellowOn();
}

void Connected(event e, void* data)
{
	Leds_GreenOn();
}

void Start()
{
	//	SensorManager_InstallSensor(&temperatureSensorMax6662Interface);
	//	SensorManager_InstallSensor(&heartRateVariabilityInterface);
	//
	//	EventDispatcher_Subscribe(SENSOR_TEMPERATURE_EAR_LEFT, ThermoUpdate);

	EventDispatcher_Subscribe(EVENT_SYNCHRONIZED, Synced);
	EventDispatcher_Subscribe(EVENT_CONNECTED, Connected);


#if MASTER_NODE == 1
	Timer_CreateConfiguration(&tim, 1000000, TIMER_MODE_RELAXED_CONTINUES, Ping);
	Timer_Start(&tim);
#endif
}
