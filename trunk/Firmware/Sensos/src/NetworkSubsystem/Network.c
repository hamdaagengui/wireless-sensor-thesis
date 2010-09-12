/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */

#include <Globals.h>

// Device statistics
typedef struct
{
	uint8_t txPower;
	uint8_t rssi;
	uint8_t energyLevel;
	uint8_t queueLevel;
	uint8_t busyLevel;
	uint8_t cost;
} deviceInfo;

deviceInfo devices[16];

// Routing
typedef struct
{
	uint8_t hops;
} routeInfo;
routeInfo routingTable[16][16];

// Frames
enum
{
	FRAME_TYPE_DATA,
	FRAME_TYPE_MANAGEMENT
};

typedef struct
{
	uint8_t source :4;
	uint8_t destination :4;
	uint8_t commands[];
} networkFrame;

// Commands
enum
{
	COMMAND_CONFIGURATION,
	COMMAND_ENERGY_LEVEL,
	COMMAND_QUEUE_LEVEL,
	COMMAND_BUSY_LEVEL,
	COMMAND_ACKNOWLEDGE,
	COMMAND_NOT_ACKNOWLEDGE,
	COMMAND_SENSOR_DATA
};

typedef struct
{
	uint8_t commandId :3;
	uint8_t parameter :5;
} baseCommand;

typedef struct
{
	uint8_t commandId :3;
	uint8_t sensorId :5;
	uint8_t data[];
} sensorDataCommand;

void Network_Initialize()
{
	for (uint8_t i = 0; i < 16; ++i)
	{
		for (uint8_t j = 0; j < 16; ++j)
		{
			if (i == j)
			{
				routingTable[i][j].hops = 0;
			}
			else
			{

			}
		}
	}
}

static void FrameReceived(uint8_t* data, uint8_t length)
{
	/*

	 update device info table

	 for me
	 handle
	 not for me
	 route?


	 */
}
