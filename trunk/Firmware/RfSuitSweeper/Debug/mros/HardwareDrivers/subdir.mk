################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mros/HardwareDrivers/GenericKeyboard.c \
../mros/HardwareDrivers/MAX7221.c \
../mros/HardwareDrivers/MatrixKeyboard.c \
../mros/HardwareDrivers/SevenSegmentDisplay.c 

OBJS += \
./mros/HardwareDrivers/GenericKeyboard.o \
./mros/HardwareDrivers/MAX7221.o \
./mros/HardwareDrivers/MatrixKeyboard.o \
./mros/HardwareDrivers/SevenSegmentDisplay.o 

C_DEPS += \
./mros/HardwareDrivers/GenericKeyboard.d \
./mros/HardwareDrivers/MAX7221.d \
./mros/HardwareDrivers/MatrixKeyboard.d \
./mros/HardwareDrivers/SevenSegmentDisplay.d 


# Each subdirectory must supply rules for building sources it contributes
mros/HardwareDrivers/%.o: ../mros/HardwareDrivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\mros" -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\src" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128rfa1 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


