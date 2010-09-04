/*
 * Config.h
 *
 *  Created on: 27/08/2010
 *      Author: Coma
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define KERNEL_MAXIMUM_NUMBER_OF_TASKS					16
#define KERNEL_TICKS_PER_SECOND									1000

#define FRAMETRANSCEIVER_TX_BUFFER_SIZE					400
#define FRAMETRANSCEIVER_RX_BUFFER_SIZE					400
#define FRAMETRANSCEIVER_MAXIMUM_FRAME_SIZE			128

// Configuration of RadioDriver
#define RADIODRIVER_AUTO_RX
#define RADIODRIVER_USE_CRC


#endif /* CONFIG_H_ */
