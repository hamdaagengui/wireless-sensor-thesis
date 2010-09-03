################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mros/Inspector/Inspector.c 

OBJS += \
./mros/Inspector/Inspector.o 

C_DEPS += \
./mros/Inspector/Inspector.d 


# Each subdirectory must supply rules for building sources it contributes
mros/Inspector/%.o: ../mros/Inspector/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\mros" -I"C:\Users\Coma\workspace_helios\RfSuitSweeper\src" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128rfa1 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


