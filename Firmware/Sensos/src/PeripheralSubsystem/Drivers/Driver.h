/*
 * Driver.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef DRIVER_H_
#define DRIVER_H_

typedef struct
{
	void (*initialize)();
	void (*powerDown)();
	void (*powerUp)();
} driver;

#endif /* DRIVER_H_ */
