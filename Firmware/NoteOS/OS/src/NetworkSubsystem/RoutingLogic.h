/*
 * RoutingLogic.h
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#ifndef ROUTINGLOGIC_H_
#define ROUTINGLOGIC_H_

#include "NetworkInternals.h"

#define NO_ROUTE																										0xff

typedef struct
{
	uint16_t costToNeighbors[15];
	bool marked;
	uint16_t cost;
	uint8_t previousNode;
} node;

extern uint8_t routingTable[15];

extern bool RoutingLogic_FindRouteToNode(uint8_t target);

#endif /* ROUTINGLOGIC_H_ */
