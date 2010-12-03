/*
 * RoutingLogic.c
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#include "RoutingLogic.h"

#define COST_INFINITY																								0xffff

#define NETWORK_UNREACHABLE																					0xffff
#define LINK_DEAD																										0

uint8_t routingTable[15] = { NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE };

typedef struct
{
	int8_t loss;
	uint8_t node;
} route_hop;

typedef struct
{
	route_hop hops[15];
} route;

uint8_t RoutingLogic_PickCheapestNode(node nodes[])
{
	uint16_t lowestCost = COST_INFINITY;
	uint8_t index = NO_ROUTE;

	for (uint8_t i = 0; i < 15; i++)
	{
		if (nodes[i].marked == false)
		{
			if (nodes[i].cost < lowestCost)
			{
				index = i;
			}
		}
	}

	return index;
}

bool RoutingLogic_FindRouteToNode(uint8_t target)
{
	node nodes[15];
	for (uint8_t i = 0; i < 15; i++)
	{
		node* n = &nodes[i];
		n->marked = false;
		n->cost = COST_INFINITY;
		n->previousNode = NO_ROUTE;
	}
	nodes[address].cost = 0;

	for (uint8_t i = 0; i < 15; i++)
	{
		uint8_t currentIndex = PickCheapestNode(nodes);
		if (currentIndex == NO_ROUTE)
		{
			routingTable[target] = NO_ROUTE;
			return false;
		}
		if (currentIndex == target)
		{
			uint8_t next = target;
			uint8_t previous = nodes[next].previousNode;
			while (previous != address)
			{
				next = previous;
				previous = nodes[next].previousNode;
			}
			routingTable[target] = next;
			return true;
		}

		node* current = &nodes[currentIndex];
		for (uint8_t neighborIndex = 0; neighborIndex < 15; neighborIndex++)
		{
			if (current->costToNeighbors[neighborIndex] != 0)
			{
				node* neighbor = &nodes[neighborIndex];
				uint16_t newCost = current->cost + current->costToNeighbors[neighborIndex];
				if (newCost < neighbor->cost)
				{
					neighbor->cost = newCost;
					neighbor->previousNode = currentIndex;
				}
			}
		}

		current->marked = true;
	}
}
