/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include <SensorSubsystem/SensorManager.h>
#include <SensorSubsystem/Sensors/HeartRateVariability.h>

void Initialize()
{

}

void Start()
{
	//	sensor_interface hrv;
	//	HeartRateVariability_Create(&hrv);
	//	SensorManager_InstallSensor(&hrv);
}

//#define yieldable																static uint8_t state = 0; uint8_t step = 0;
//#define yield_if(condition)											{ if(!(condition)) return; state++; } }
//#define yieldbreak															{ state = 0; return; } }
//#define yieldblock															if(state == step++) {
//
//
//uint8_t counter = 0;
//
//void Bla()
//{
//	yieldable
//
//	yield_if(counter < 10)
//
//}
