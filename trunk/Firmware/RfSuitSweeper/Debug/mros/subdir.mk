################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mros/Checksum.c \
../mros/FIFO.c \
../mros/Globals.c \
../mros/Heap.c \
../mros/Memory.c \
../mros/Settings.c \
../mros/Strings.c 

OBJS += \
./mros/Checksum.o \
./mros/FIFO.o \
./mros/Globals.o \
./mros/Heap.o \
./mros/Memory.o \
./mros/Settings.o \
./mros/Strings.o 

C_DEPS += \
./mros/Checksum.d \
./mros/FIFO.d \
./mros/Globals.d \
./mros/Heap.d \
./mros/Memory.d \
./mros/Settings.d \
./mros/Strings.d 


# Each subdirectory must supply rules for building sources it contributes
mros/%.o: ../mros/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\mros" -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\src" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128rfa1 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


