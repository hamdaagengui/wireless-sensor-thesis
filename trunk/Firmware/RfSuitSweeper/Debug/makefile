################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include mros/subdir.mk
-include mros/StateMachine/subdir.mk
-include mros/Slicer/subdir.mk
-include mros/Peripherals/subdir.mk
-include mros/Network/uStack/subdir.mk
-include mros/Kernel/subdir.mk
-include mros/Inspector/subdir.mk
-include mros/HardwareDrivers/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
RfSuitSweeper.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: RfSuitSweeper.elf secondary-outputs

# Tool invocations
RfSuitSweeper.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,RfSuitSweeper.map -mmcu=atmega128rfa1 -o"RfSuitSweeper.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

RfSuitSweeper.lss: RfSuitSweeper.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S RfSuitSweeper.elf  >"RfSuitSweeper.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: RfSuitSweeper.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128rfa1 RfSuitSweeper.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) RfSuitSweeper.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
