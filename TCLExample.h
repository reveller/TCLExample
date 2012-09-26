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
//    ButtonMenu    A5        button3
//    ButtonDN      A6        button2
//    ButtonUP      A7        button1


#define RELAY_1    2
#define RELAY_2    10


// DHT11 Temp/Humdity Sensor is plugged into port 11
#define DHT11_PIN 11
// init the DHT11 sensor
extern dht11 DHT;


// variables for maintaining button status
extern byte buttonSetting;  // Default setting

#define CLOCK_RESOLUTION_MS 200

// Service Intervals
#define DS_INTERVAL  (10*CLOCK_RESOLUTION_MS)
#define DHT_INTERVAL (10*CLOCK_RESOLUTION_MS)
#define RLY_INTERVAL (10*CLOCK_RESOLUTION_MS)
// Original called for this every 200ms
#define updateTemperatures_INTERVAL (1*CLOCK_RESOLUTION_MS)
#define updateSlowFilteredTemperatures_INTERVAL (10*CLOCK_RESOLUTION_MS)
#define updateSlope_INTERVAL (60*CLOCK_RESOLUTION_MS)

#define STARTUP_TEMP_DEFAULT 25

// Service Flags
#define DS_SERVICE                             B00000001
#define DHT_SERVICE                            B00000010
#define RLY_SERVICE                            B00000100
#define CLK_SERVICE                            B00001000
#define updateTemperatures_SERVICE             B00010000
#define updateSlowFilteredTemperatures_SERVICE B00100000
#define updateSlope_SERVICE                    B0100000

extern byte Services;

//extern byte DS_Interval;
//extern byte DHT_Interval;
//extern byte RLY_Interval;
//extern byte updateTemperatures_Interval;
//extern byte updateSlowFilteredTemperatures_Interval;
//extern byte updateSlope_Interval;

// init the OLED
extern OLEDFourBit lcd;

//end of add your includes here

#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
void timerISR();
//void DSControl();
//void DHTControl();
void RLYControl();
void CLKControl();
//void updateTemperatures();
//void updateSlowFilteredTemperatures();
//void updateSlope();
#ifdef __cplusplus
} // extern "C"
#endif

//Do not add code below this line
#endif /* TLCExample_H_ */
