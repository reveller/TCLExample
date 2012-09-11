// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef TLCExample_H_
#define TLCExample_H_

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht11.h>
#include <OLEDFourBit.h>
#include <MsTimer2.h>

//add your includes for the project TLCExample here

//                  Arduino   UberFridge
//  1 OLED GND      GND       OLED GND
//  2 OLED 5V	    Vdd (5v)  OLED 5V
//    RLYIN1        D2
//  4 OLED RS       D3        OLED RS
//  5 OLED RW       D4        OLED RW
//  6 OLED E        D5        OLED E
// 11 OLED DB4      D6        OLED DB4
// 12 OLED DB5      D7        OLED DB5
// 13 OLED DB6      D8        OLED DB6
// 14 OLED DB7      D9        OLED DB7
//    RLYIN2        D10       coolingPin - OUTPUT
//    DHT11 Sig     D11       heatingPin - OUTPUT
//    DS18B20 DQ    D12       doorPin    - INPUT - ONE WIRE BUS
//                  D13
//                  A0        beerPin
//                  A1        fridgePin
//                  A2
//                  A3
//                  A4
//                  A5        button3
//    Button2       A6        button2
//    Button1       A7        button1


#define RELAY_1    2
#define RELAY_2    10

// Data wire is plugged into port 12 on the Arduino
#define ONE_WIRE_BUS 12

// DHT11 Temp/Humdity Sensor is plugged into port 11
#define DHT11_PIN 11
// init the DHT11 sensor
extern dht11 DHT;

#define button1 A7  // 10
#define button2 A6  // 13

#define BUTTON_BOUNCE 250 	// millis to wait to inc setting on continuous button press

// variables for maintaining button status
extern int  button1State;
extern int  button2State;
extern byte buttonSetting;  // Default setting

// Service Intervals
#define DS_INTERVAL  1
#define DHT_INTERVAL 1
#define BTN_INTERVAL 10

// Service Flags
#define DS_SERVICE  B00000001
#define DHT_SERVICE B00000010
#define BTN_SERVICE B00000100
#define CLK_SERVICE B00001000

extern byte Services;

extern byte DS_Interval;
extern byte DHT_Interval;
extern byte BTN_Interval;

// init the OLED
extern OLEDFourBit lcd;

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
extern OneWire oneWire;

// Pass our oneWire reference to Dallas Temperature.
extern DallasTemperature sensors;



//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
void timerISR();
void DSControl();
void DHTControl();
void buttonControl();
void CLKControl();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project TLCExample here




//Do not add code below this line
#endif /* TLCExample_H_ */
