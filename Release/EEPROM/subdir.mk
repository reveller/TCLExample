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
	avr-g++ -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/Arduino_Nano_w__ATmega328/arduino" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/Arduino_Nano_w__ATmega328/eightanaloginputs" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TLCExample" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/dallas-temperature-control" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/OneWire" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/DHT11" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/OLEDFourBit" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/MsTimer2" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/Time" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/DebugUtils" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/EEPROM" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/OLEDFourBit11" -I"C:/Program Files/arduino-1.0.1/libraries/dallas-temperature-control" -I"C:/Program Files/arduino-1.0.1/libraries" -I"C:/Program Files/arduino-1.0.1/libraries/OneWire" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/OneWire/OneWire.h" -I"C:/Program Files/arduino-1.0.1/hardware/arduino/cores/arduino" -I../TCLExample/Arduino_Nano_w__ATmega328/arduino -I../TCLExample/Arduino_Nano_w__ATmega328/eightanaloginputs -I../TCLExample/TCLExample/dallas-temperature-control -I"C:/Program Files/arduino-1.0.1/hardware/arduino/variants/eightanaloginputs" -I../TCLExample/TCLExample/OneWire -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/TCLExample/TCLExample/TCLExample/OLEDFourBit11" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/OLEDFourBit" -I"C:/Users/sfeltner/My Projects/arduino/TCLExample/Users/sfeltner/My Projects/arduino/TCLExample" -I"C:\Program Files\arduino-1.0.1\libraries\MsTimer2" -D__IN_ECLIPSE__=1 -DARDUINO=101 -DUSB_PID= -DUSB_VID= -Wall -Os -ffunction-sections -fdata-sections -fno-exceptions -g -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -c -o "$@" -x c++ "$<"
	@echo 'Finished building: $<'
	@echo ' '


