################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include dallas-temperature-control/subdir.mk
-include Time/subdir.mk
-include OneWire/subdir.mk
-include OLEDFourBit/subdir.mk
-include MsTimer2/subdir.mk
-include EEPROM/subdir.mk
-include DHT11/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
TCLExample.lss \

FLASH_IMAGE += \
TCLExample.hex \

EEPROM_IMAGE += \
TCLExample.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: TCLExample.elf secondary-outputs

# Tool invocations
TCLExample.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc -Os -Wl,--gc-sections  -L"C:\Users\sfeltner\My Projects\arduino\Arduino_Nano_w__ATmega328/Release" -mmcu=atmega328p  -o"TCLExample.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '

TCLExample.lss: TCLExample.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S TCLExample.elf  >"TCLExample.lss"
	@echo 'Finished building: $@'
	@echo ' '

TCLExample.hex: TCLExample.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex TCLExample.elf  "TCLExample.hex"
	@echo 'Finished building: $@'
	@echo ' '

TCLExample.eep: TCLExample.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex TCLExample.elf  "TCLExample.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: TCLExample.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p TCLExample.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(INO_DEPS)$(CPP_DEPS)$(PDE_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) TCLExample.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
