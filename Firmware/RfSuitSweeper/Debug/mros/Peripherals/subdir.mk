################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mros/Peripherals/CAN.c \
../mros/Peripherals/SPI.c \
../mros/Peripherals/UART0.c \
../mros/Peripherals/UART1.c 

OBJS += \
./mros/Peripherals/CAN.o \
./mros/Peripherals/SPI.o \
./mros/Peripherals/UART0.o \
./mros/Peripherals/UART1.o 

C_DEPS += \
./mros/Peripherals/CAN.d \
./mros/Peripherals/SPI.d \
./mros/Peripherals/UART0.d \
./mros/Peripherals/UART1.d 


# Each subdirectory must supply rules for building sources it contributes
mros/Peripherals/%.o: ../mros/Peripherals/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\mros" -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\src" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128rfa1 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


