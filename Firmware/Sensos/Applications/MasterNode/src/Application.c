/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include <Globals.h>
#include <HardwareAbstractionLayer/RadioDriver.h>
#include <EventSubsystem/EventDispatcher.h>

void Sensors_RegisterSensor();

void SensorDataHandler(eventReport* e)
{

}

void Start()
{


	//	Adc_Activate();

	//sensor thermo;
	//thermo.bandwidth

	//	Sensors_RegisterSensor();

	EventDispatcher_RegisterSubscriber(EVENT_SENSOR_DATA, SensorDataHandler);

}

/*


 void Start()
 {
 enable peripherals
 register sensors
 register event listeners
 }



 */
