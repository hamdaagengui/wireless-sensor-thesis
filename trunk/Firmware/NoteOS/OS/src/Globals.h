#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>
#include <util/atomic.h>
#include "DefaultConfiguration.h"

#define ReadBit(var, bit)												(((var) & (1 << (bit))) != 0)
#define SetBit(var, bit)												((var) |= (1 << (bit)))
#define ClearBit(var, bit)											((var) &= ~(1 << (bit)))
#define ToggleBit(var, bit)											((var) ^= (1 << (bit)))

#define Modify(var, mask, value)								(var = ((var & ~mask) | (value & mask)))

enum
{
	false,
	true
};
typedef uint8_t bool;

#define Critical()															cli()
#define NonCritical()														sei()
#define Nop()																		__asm__ volatile("nop")
#define lengthof(x)															( sizeof(x) / sizeof(x[0]) )

#define MemoryBarrier()													__asm__ volatile ("" : : : "memory")

typedef void (*block_handler)(void* data, uint8_t length);
typedef void* (*bidirectional_block_handler)(void* data, uint8_t length);
typedef void (*notification_handler)();
typedef void (*result_handler)(bool succes);
enum
{
	RESULT_OK,
	RESULT_ERROR
};

///#define inline __attribute__ ((__always_inline__))
///#define a  flatten

#endif /* GLOBALS_H_ */
