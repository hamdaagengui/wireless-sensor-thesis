/*
 * PeripheralDataDistributor.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef PERIPHERALDATADISTRIBUTOR_H_
#define PERIPHERALDATADISTRIBUTOR_H_

void PDD_Initialize();
void PDD_Run();

void PDD_Subscribe(uint8_t peripheral);

void* PDD_AcquireDetailsObject();
void PDD_Publish(uint8_t peripheral, void* details);

#endif /* PERIPHERALDATADISTRIBUTOR_H_ */
