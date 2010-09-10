/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */

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

typedef struct
{
	uint8_t destination :8;
	uint8_t source :8;
	uint8_t frameType :8;
} baseFrame;

typedef struct
{
	uint8_t destination :8;
	uint8_t source :8;
	uint8_t frameType :8;
	uint8_t
} baseFrame;

void Network_Initialize()
{

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
