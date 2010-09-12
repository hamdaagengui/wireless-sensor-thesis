/*
 * PeripheralManager.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef PERIPHERALMANAGER_H_
#define PERIPHERALMANAGER_H_

/*

- Initialize peripherals -> power down
                            power up

- Register subscribers

 */

void PeripheralManager_Initialize();
void PeripheralManager_Configure();
void PeripheralManager_Start();

#endif /* PERIPHERALMANAGER_H_ */
