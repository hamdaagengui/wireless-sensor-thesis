/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include <SensorSubsystem/SensorManager.h>
#include <SensorSubsystem/Sensors/HeartRateVariability.h>
#include <Kernel/Timer.h>
#include <NetworkSubsystem/Network.h>
#include <HardwareAbstractionLayer/HardwareAbstractionLayer.h>

#ifdef MASTER_NODE
timer_configuration tickerTimer;

void Ticker()
{
	static uint8_t ticker = 0;

//	Network_SendData(2, &ticker, 1);

	if (ticker & 1)
	{
		Leds_YellowOn();
	}
	else
	{
		Leds_YellowOff();
	}

	ticker++;
}
#endif

void Initialize()
{

}

void Start()
{
	//	sensor_interface hrv;
	//	HeartRateVariability_Create(&hrv);
	//	SensorManager_InstallSensor(&hrv);

#ifdef MASTER_NODE
	Timer_CreateConfiguration(&tickerTimer, 1000000, TIMER_MODE_RELAXED_CONTINUES, Ticker);
	Timer_Start(&tickerTimer);
#endif
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
