/*
 * main.c
 *
 *  Created on: 09/09/2010
 *      Author: coma
 */

#include <stdint.h>

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

void HandleEvent(event e)
{

}

int main()
{

	while (1)
	{

	}
}
