################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Program\ Files/arduino-1.0.1/libraries/EEPROM/EEPROM.cpp 

OBJS += \
./EEPROM/EEPROM.o 

CPP_DEPS += \
./EEPROM/EEPROM.d 


# Each subdirectory must supply rules for building sources it contributes
EEPROM/EEPROM.o: C:/Program\ Files/arduino-1.0.1/libraries/EEPROM/EEPROM.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\Program Files\arduino-1.0.1\hardware\arduino\cores\arduino" -I"C:\Program Files\arduino-1.0.1\hardware\arduino\variants\eightanaloginputs" -I"C:\Program Files\arduino-1.0.1\libraries\dallas-temperature-control" -I"C:\Program Files\arduino-1.0.1\libraries\OneWire" -I"C:\Program Files\arduino-1.0.1\libraries\DHT11" -I"C:\Program Files\arduino-1.0.1\libraries\OLEDFourBit" -I"C:\Program Files\arduino-1.0.1\libraries\MsTimer2" -I"C:\Program Files\arduino-1.0.1\libraries\Time" -I"C:\Program Files\arduino-1.0.1\libraries\DebugUtils" -I"C:\Program Files\arduino-1.0.1\libraries\EEPROM" -D__IN_ECLIPSE__=1 -DUSB_VID= -DUSB_PID= -DARDUINO=101 -Wall -Os -ffunction-sections -fdata-sections -fno-exceptions -g -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -c -o "$@" -x c++ "$<"
	@echo 'Finished building: $<'
	@echo ' '


