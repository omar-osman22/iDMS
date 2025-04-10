################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
CodeCrafters.lss \

FLASH_IMAGE += \
CodeCrafters.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: CodeCrafters.elf secondary-outputs

# Tool invocations
CodeCrafters.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,CodeCrafters.map -mmcu=atmega128 -o "CodeCrafters.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

CodeCrafters.lss: CodeCrafters.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S CodeCrafters.elf  >"CodeCrafters.lss"
	@echo 'Finished building: $@'
	@echo ' '

CodeCrafters.hex: CodeCrafters.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex CodeCrafters.elf  "CodeCrafters.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: CodeCrafters.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128 CodeCrafters.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) CodeCrafters.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
