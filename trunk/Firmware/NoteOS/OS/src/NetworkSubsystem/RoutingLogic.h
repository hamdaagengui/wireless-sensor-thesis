/*
 * RoutingLogic.h
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#ifndef ROUTINGLOGIC_H_
#define ROUTINGLOGIC_H_

#include "NetworkInternals.h"

#define ROUTE_INVALID																								255

typedef struct
{
	uint16_t costToNeighbors[15];
	bool marked;
	uint16_t costFromStart;
	uint8_t previousNode;
} node;

#define RSI_TIME_REMAINING_START_TIME																255
#define RSI_TIME_REMAINING_THRESHOLD_TIME														100

typedef struct
{
	uint8_t timeRemaining;
	int8_t transmissionPower;
	int8_t rssi;
	int8_t linkLoss;
} route_significant_information;

extern uint8_t routingTable[16];
extern route_significant_information rsi[15];
extern node nodes[15];

extern void RoutingLogic_CalculateCosts();
extern void RoutingLogic_FindRouteToNode(uint8_t target);

#endif /* ROUTINGLOGIC_H_ */
