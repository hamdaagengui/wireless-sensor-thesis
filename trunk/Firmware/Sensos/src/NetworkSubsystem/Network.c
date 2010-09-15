/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */

#include "Network.h"
#include "HardwareAbstractionLayer/RadioDriver.h"

uint8_t assignedId;

enum
{
	STATE_UNCONNECTED,
	STATE_SYNCHRONIZING,
	STATE_CONNECTED
};
uint8_t state = STATE_UNCONNECTED;

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
	COMMAND_ROUTES,
	COMMAND_ENERGY_LEVEL,
	COMMAND_QUEUE_LEVEL,
	COMMAND_BUSY_LEVEL,
	COMMAND_ACKNOWLEDGE,
	COMMAND_SENSOR_DATA
};

typedef struct
{
	uint8_t commandId :3;
	uint8_t reserved :5;
} baseCommand;

typedef struct
{
	uint8_t commandId :3;
	uint8_t level :5;
} configurationCommand;

typedef struct
{
	uint8_t commandId :3;
	uint8_t reserved :5;
	uint8_t hops[8];
} routesCommand;

typedef struct
{
	uint8_t commandId :3;
	uint8_t level :5;
} energyLevelCommand;

typedef struct
{
	uint8_t commandId :3;
	uint8_t level :5;
} queueLevelCommand;

typedef struct
{
	uint8_t commandId :3;
	uint8_t level :5;
} busyLevelCommand;

typedef struct
{
	uint8_t commandId :3;
	uint8_t acknowledge :1;
	uint8_t reserved :4;
} acknowledgeCommand;

typedef struct
{
	uint8_t commandId :3;
	uint8_t reserved :5;
	uint8_t sensorNode :4;
	uint8_t sensorId :4;
	uint8_t length;
	uint8_t data[];
} sensorDataCommand;

static void FrameReceived(uint8_t* data, uint8_t length);

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

	RadioDriver_Initialize(FrameReceived);
}

void Network_Send(uint8_t channel, void* data, uint8_t length)
{

}

static void FrameReceived(uint8_t* data, uint8_t length)
{
	uint8_t source = ((networkFrame*) data)->source;
	uint8_t destination = ((networkFrame*) data)->destination;

	for (uint8_t i = 1; i < length;) // process all commands in frame
	{
		baseCommand* bc = (baseCommand*) &data[i];

		switch (bc->commandId)
		{
			case COMMAND_CONFIGURATION:
				{
					configurationCommand* c = (configurationCommand*) bc;
					i += sizeof(configurationCommand);
				}
				break;

			case COMMAND_ROUTES:
				{
					routesCommand* c = (routesCommand*) bc;
					i += sizeof(routesCommand);
				}
				break;

			case COMMAND_ENERGY_LEVEL:
				{
					energyLevelCommand* c = (energyLevelCommand*) bc;
					i += sizeof(energyLevelCommand);
				}
				break;

			case COMMAND_QUEUE_LEVEL:
				{
					queueLevelCommand* c = (queueLevelCommand*) bc;
					i += sizeof(queueLevelCommand);
				}
				break;

			case COMMAND_BUSY_LEVEL:
				{
					busyLevelCommand* c = (busyLevelCommand*) bc;
					i += sizeof(busyLevelCommand);
				}
				break;

			case COMMAND_ACKNOWLEDGE:
				{
					sensorDataCommand* c = (sensorDataCommand*) bc;
					i += sizeof(sensorDataCommand);
				}
				break;

			case COMMAND_SENSOR_DATA:
				{
					sensorDataCommand* c = (sensorDataCommand*) bc;
					i += sizeof(sensorDataCommand);
				}
				break;

			default: // error -> abort
				i = length;
				break;
		}
	}


	/*

	 update device info table

	 for me
	 handle
	 not for me
	 route?

	 */
}
