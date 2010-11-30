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
	network_routes_packet* p = Network_CreateBedPacket(12, 15, sizeof(network_routes_packet));

	Network_QueueBedPacket();
	Leds_RedToggle();
}

timer_configuration tim;
void Start()
{
	//	SensorManager_InstallSensor(&temperatureSensorMax6662Interface);
	//	SensorManager_InstallSensor(&heartRateVariabilityInterface);
	//
	//	EventDispatcher_Subscribe(SENSOR_TEMPERATURE_EAR_LEFT, ThermoUpdate);


#if MASTER_NODE == 1
	Timer_CreateConfiguration(&tim, 1000000, TIMER_MODE_RELAXED_CONTINUES, Ping);
	Timer_Start(&tim);
#endif
}
