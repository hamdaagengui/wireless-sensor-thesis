/*
 * main.c
 *
 *  Created on: 09/09/2010
 *      Author: coma
 */

#include <Globals.h>

typedef void (*eventHandler)();

enum
{
	PERIPHERAL_ADC,
	PERIPHERAL_SPI,
	PERIPHERAL_IIC
};

typedef struct
{
	uint16_t samplesPerSecond;
} adcConfiguration;

typedef struct
{
	uint8_t peripheralId;
	eventHandler handler;
	void* configuration;
} peripheralRequest;

enum
{
	EVENT_ADC,
	EVENT_INT1
};

typedef struct
{
	uint8_t id;
} event;

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

void HandleEvent(event e)
{

}

void Kernel_Initialize();
void Network_Initialize();
void Sensors_RegisterSensor();

int main()
{
	Kernel_Initialize();
	Network_Initialize();

	sensor thermo;
	//thermo.bandwidth

	Sensors_RegisterSensor();

	while (1)
	{

	}
}

/*


 void Start()
 {
 enable peripherals
 register sensors
 register event listeners
 }



*/
