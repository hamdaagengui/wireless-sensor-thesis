#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>
#include <util/atomic.h>
#include <Configuration.h>

#define ReadBit(var, bit)												(((var) & (1 << (bit))) != 0)
#define SetBit(var, bit)												((var) |= (1 << (bit)))
#define ClearBit(var, bit)											((var) &= ~(1 << (bit)))
#define ToggleBit(var, bit)											((var) ^= (1 << (bit)))

#define Modify(var, mask, value)								(var = ((var & ~mask) | (value & mask)))

typedef enum
{
	false,
	true
} bool;

#define Critical()															cli()
#define NonCritical()														sei()
#define Nop()																		__asm__ volatile("nop")
#define lengthof(x)															( sizeof(x) / sizeof(x[0]) )

#define MemoryBarrier()													__asm__ volatile ("" : : : "memory")

typedef void (*completion_handler)();
//typedef void (*completion_handler)(void* operation);
typedef void (*result_handler)(bool succes);
typedef void (*block_handler)(void* data, uint8_t length);
typedef void* (*bidirectional_block_handler)(void* data, uint8_t length);

///#define inline __attribute__ ((__always_inline__))
///#define a  flatten

extern uint8_t serialNumber;

#endif /* GLOBALS_H_ */
