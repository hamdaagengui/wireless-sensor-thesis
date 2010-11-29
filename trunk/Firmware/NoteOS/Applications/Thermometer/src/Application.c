/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "NoteOS.h"

#ifdef MASTER_NODE
timer_configuration tickerTimer;
timer_configuration pulseTimer;

timer_configuration analogTickerConfiguration;
adc_configuration analogConfiguration;
uint16_t analogValue;

void AnalogTicker()
{
	ADC_Convert(&analogConfiguration);

	static uint16_t counter = 0;

	if (++counter >= analogValue)
	{
		counter = 0;
		Leds_GreenToggle();
	}
}

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

void ThermoUpdate(uint8_t eventId, void* data)
{
	int16_t* v = Network_CreateSensorDataPacket(0, eventId, 2);
	*v = *((int16_t*) data);
	Network_SendPacket();
}

void Start()
{
#ifdef MASTER_NODE
	Timer_CreateConfiguration(&tickerTimer, 1000000, TIMER_MODE_RELAXED_CONTINUES, Ticker);
	Timer_Start(&tickerTimer);

	Timer_CreateConfiguration(&pulseTimer, 500000, TIMER_MODE_RELAXED_ONE_SHOT, PulseOff);

	Timer_CreateConfiguration(&analogTickerConfiguration, 20000, TIMER_MODE_RELAXED_CONTINUES, AnalogTicker);
	Timer_Start(&analogTickerConfiguration);
	ADC_CreateConfiguration(&analogConfiguration, 0, ADC_PRESCALER_128, &analogValue, NULL);
	ADC_Convert(&analogConfiguration);
#endif

	SensorManager_InstallSensor(&thermomoterSpiInterface);
	SensorManager_InstallSensor(HeartRateVariability_GetInterface());

	EventDispatcher_Subscribe(EVENT_SENSOR_DATA, ThermoUpdate);
}
