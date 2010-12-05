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

static uint8_t PickCheapestNode(node nodes[]);

uint8_t routingTable[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, BROADCAST_ADDRESS };
route_significant_information rsi[15];
node nodes[15];

void RoutingLogic_CalculateCosts()
{
	for (uint8_t i = 0; i < 15; i++)
	{
		if (i != address)
		{
			nodes[address].costToNeighbors[i] = rsi[i].rssi;
		}
	}
}

void RoutingLogic_FindRouteToNode(uint8_t target)
{
	for (uint8_t i = 0; i < 15; i++)
	{
		nodes[i].marked = false;
		nodes[i].costFromStart = COST_INFINITY;
		nodes[i].previousNode = ROUTE_INVALID;
	}
	nodes[address].costFromStart = 0;

	for (uint8_t i = 0; i < 15; i++)
	{
		uint8_t currentIndex = PickCheapestNode(nodes);
		if (currentIndex == ROUTE_INVALID)
		{
			routingTable[target] = ROUTE_INVALID;
			return;
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
			return;
		}

		node* current = &nodes[currentIndex];
		for (uint8_t neighborIndex = 0; neighborIndex < 15; neighborIndex++)
		{
			if (current->costToNeighbors[neighborIndex] != 0)
			{
				node* neighbor = &nodes[neighborIndex];
				uint16_t newCost = current->costFromStart + current->costToNeighbors[neighborIndex];
				if (newCost < neighbor->costFromStart)
				{
					neighbor->costFromStart = newCost;
					neighbor->previousNode = currentIndex;
				}
			}
		}

		current->marked = true;
	}
}

static uint8_t PickCheapestNode(node nodes[])
{
	uint16_t lowestCost = COST_INFINITY;
	uint8_t index = ROUTE_INVALID;

	for (uint8_t i = 0; i < 15; i++)
	{
		if (nodes[i].marked == false)
		{
			if (nodes[i].costFromStart < lowestCost)
			{
				index = i;
			}
		}
	}

	return index;
}
