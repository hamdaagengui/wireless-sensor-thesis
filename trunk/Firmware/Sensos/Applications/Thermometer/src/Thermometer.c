/*
 * Thermometer.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include <Globals.h>

enum
{
	EVENT_ADC,
	EVENT_INT1
};

typedef struct
{

} eventListener;

eventListener eventListeners[16];

enum
{
	FLOW_SPONTANEOUS,
	FLOW_CONSTANT,
	FLOW_BURSTY
};

typedef struct
{
	uint8_t id;
	uint32_t bandwidth;
	uint8_t flow;
	uint8_t maxLoss;
} sensor;

void Sensors_RegisterSensor();

void Start()
{

	sensor thermo;
	//thermo.bandwidth

	//	Sensors_RegisterSensor();

}

/*


 void Start()
 {
 enable peripherals
 register sensors
 register event listeners
 }



 */
