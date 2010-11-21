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
#include <PlatformAbstractionLayer/PlatformAbstractionLayer.h>

#ifdef MASTER_NODE
timer_configuration tickerTimer;
timer_configuration pulseTimer;

void Ticker()
{
	static uint8_t ticker;
	static uint8_t counter;

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

	if (++counter >= 4)
	{
		Leds_RedOn();
		Timer_Start(&pulseTimer);
		counter = 0;
	}
	//	if (counter == 0)
	//	{
	//		Leds_RedOn();
	//		Timer_Start(&tickerTimer2);
	//	}
	//	else if (counter == 10)
	//	{
	//		Leds_RedOff();
	//		Timer_Stop(&tickerTimer2);
	//	}
	//
	//	if (++counter >= 20)
	//	{
	//		counter = 0;
	//	}
}

void PulseOff()
{
	Leds_RedOff();
}
#endif

void Start()
{
	//	sensor_interface hrv;
	//	HeartRateVariability_Create(&hrv);
	//	SensorManager_InstallSensor(&hrv);

#ifdef MASTER_NODE
	Timer_CreateConfiguration(&tickerTimer, 1000000, TIMER_MODE_RELAXED_CONTINUES, Ticker);
	Timer_Start(&tickerTimer);

	Timer_CreateConfiguration(&pulseTimer, 500000, TIMER_MODE_RELAXED_ONE_SHOT, PulseOff);
#endif
}
