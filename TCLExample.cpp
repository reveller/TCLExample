#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht11.h>
#include <OLEDFourBit.h>
#include <MsTimer2.h>
#include <Time.h>
#include "TCLExample.h"
#include "Clock.h"
#include "Button.h"
#include "Relay.h"
#include "TempSensors.h"

//#define DEBUG
#include <DebugUtils.h>

dht11 DHT;

#define ONE_WIRE_BUS 12

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);



#define BUTTON1_PIN A7  // 10
#define BUTTON2_PIN A6  // 13

Button button1(A6);
Button button2(A7);

byte buttonSetting = 25;  // Default setting

byte Services      = 0;

byte DS_Interval   = 0;
byte DHT_Interval  = 0;
byte RLY_Interval  = 0;

Relay relay1(RELAY_1, OFF);
Relay relay2(RELAY_2, OFF);

// init the OLED
OLEDFourBit lcd(3, 4, 5, 6, 7, 8, 9);

TempSensors *fridgeSensor;
TempSensors *beerSensor;

//celsius to fahrenheit conversion
float c2f(float val){
  float aux = (val * 9 / 5);
  return (aux + 32);
}

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}


void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // initialize the button pins as an input:

  // analog pins are input by default
  analogReference(2); //set analog reference to internal 1.1V
  delay(100);

  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();

  // Start up the DS18B20 library
  sensors.begin();

  // Set DS18B20 resolution to:
  //   9bit = 0.5C,    93.75ms time to convert (tCONV/8)
  //  10bit = 0.25C,  187.5ms  time to convert (tCONV/4)
  //  11bit = 0.125C  375ms    time to convert (tCONV/2)
  //  12bit = 0.0625C 750ms    time to convert
  sensors.setResolution(10);

//  TempSensors fridgeSensor("Fridge", 0, &sensors);
//  TempSensors beerSensor("Beer", 0, &sensors);

  fridgeSensor = new TempSensors("Fridge", 0, &sensors);
  beerSensor   = new TempSensors("Beer", 1, &sensors);

  uint8_t dsDevAddress[8];
  uint8_t dsDevCount = 0;

  dsDevCount = sensors.getDeviceCount();
  Serial.print("DS18B20 Devices Found on OneWire Bus: ");
  Serial.println(dsDevCount);
  for (int idx = 0; idx < dsDevCount; idx++){
    if (sensors.getAddress(dsDevAddress, idx)){
    	Serial.print("DS Device Address[");
    	Serial.print(idx);
    	Serial.print("]:");
    	for (int i = 0; i < 8; i++) {
    	    Serial.print(dsDevAddress[i], HEX);
    	    Serial.print(" ");
    	}
    	Serial.println();
    }
  }

  Serial.print("fridgeSensor.Name: ");
  Serial.println(fridgeSensor->Name);
  Serial.print("beerSensor.Name: ");
  Serial.println(beerSensor->Name);

  lcd.begin(20, 4);
  lcd.clear();
  lcd.home();

  lcd.setCursor(0,0);
  lcd.print("DHT-DS TEST PROGRAM ");

  lcd.display();		// Make sure the display is turned on!

  // Set interval counters
  DS_Interval  = DS_INTERVAL;
  DHT_Interval = DHT_INTERVAL;
  RLY_Interval = RLY_INTERVAL;

  // fire up the timer interrupt!
  MsTimer2::set(1000, timerISR);
  MsTimer2::start();


  Services = DS_SERVICE | DHT_SERVICE | DHT_SERVICE;
}

void timerISR(){
	Services |= CLK_SERVICE;			// service Clock once a second

    if (--DS_Interval <= 0){
    	Services |= DS_SERVICE;		// service DS18B20 once a second
        DS_Interval = DS_INTERVAL;	// reset service interval
    }
    if (--DHT_Interval <= 0){
    	Services |= DHT_SERVICE;		// service DHT11 once a second
    	DHT_Interval = DHT_INTERVAL;	// reset service interval
    }
	if (--RLY_Interval <= 0){
    	Services |= RLY_SERVICE;	// service DHT11 once a second
    	RLY_Interval = RLY_INTERVAL;	// reset service interval
    }
}

void loop(void)
{
  PROFILER1_START("Services Start");
  if (Services & CLK_SERVICE){
	  CLKControl();
  }
  if (Services & DS_SERVICE){
	  DSControl();
  }
//  if (Services & DHT_SERVICE){
//	  DHTControl();
//  }
  if (Services & RLY_SERVICE){
	  RLYControl();
  }
  PROFILER1_END("Services End");

  if (button1.IsPressed())
	  buttonSetting++;

  if (button2.IsPressed())
	  buttonSetting--;

  lcd.setCursor(0,1);
  lcd.print("Setting: ");
  lcd.print(buttonSetting);

  if(Serial.available()){
    processSyncMessage();
  }
}


void CLKControl(){
  if(timeStatus() != timeNotSet){  // here if the time has been set
    lcd.setCursor(12,1);
    OLEDClockDisplay();
  }
  Services &= ~CLK_SERVICE;		// reset the service flag
}

void DSControl(){

//  sensors.requestTemperatures(); // Send the command to get temperatures
//
//  fridgeTempC = sensors.getTempCByIndex(0);
//  beerTempC   = sensors.getTempCByIndex(0);

  fridgeSensor->GetTemperature();
  beerSensor->GetTemperature();

//  fridgeTempC = THISsensors.GetTemperature(fridgeSensor);
//  beerTempC   = sensors.GetTemperatures(sensors::SENSORS_NAME::beerSensor);

  //  Serial.print("Temperature for the device 1 (index 0) is: ");
//  Serial.println(tempC);

////  tempF = c2f(tempC);

//  Serial.print("Temp = ");
//  Serial.print(fridgeTempC);
//  Serial.println(" C ");
//  Serial.print(tempF);
//  Serial.println(" F");

  fridgeSensor->SerialPrintTemp();
  lcd.setCursor(0,2);
  fridgeSensor->LcdPrintTemp(&lcd);
//  lcd.print(fridgeSensor->Name);
//  lcd.print(" ");
//  lcd.print(fridgeSensor->CurrentTemp);
//  lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111

  beerSensor->SerialPrintTemp();
  lcd.setCursor(0,3);
  beerSensor->LcdPrintTemp(&lcd);
//  lcd.print(beerSensor->Name);
//  lcd.print(" ");
//  lcd.print(beerSensor->CurrentTemp);
//  lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111

  Services &= ~DS_SERVICE;		// reset the service flag
}


void DHTControl(){
//  Serial.print("DHT11, \t");
  lcd.setCursor(0,3);
  int chk = DHT.read(DHT11_PIN);	// Read data from DHT11
  switch (chk){
    case DHTLIB_OK:
//		Serial.print("OK,\t");
		lcd.print("DHTOK ");
//		Serial.print(DHT.humidity,1);
		lcd.print(DHT.humidity);
		lcd.print('%');
//		Serial.print(",\t");
//		Serial.print(DHT.temperature,1);
		lcd.print(" ");
		lcd.print(DHT.temperature);
		lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111
//		Serial.print(",\t");
//		Serial.println(Fahrenheit(DHT.temperature),2);
  	    lcd.print(" ");
	    lcd.print(Fahrenheit(DHT.temperature));
		lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111
		break;
    case DHTLIB_ERROR_CHECKSUM:
//		Serial.println("Checksum error,\t");
		lcd.print("Checksum error");
		break;
    case DHTLIB_ERROR_TIMEOUT:
//		Serial.println("Time out error,\t");
		lcd.print("Time out error");
		break;
    default:
//		Serial.println("Unknown error,\t");
		lcd.print("Unknown error");
		break;
  }
  Services &= ~DHT_SERVICE;		// reset the service flag
}

void RLYControl(){

  if(buttonSetting > fridgeSensor->CurrentTemp){
	  relay1.SetState(ON);
	  relay2.SetState(OFF);
  }
  else if(buttonSetting < fridgeSensor->CurrentTemp){
	  relay1.SetState(OFF);
	  relay2.SetState(ON);
  }
  else{
	  relay1.SetState(OFF);
	  relay2.SetState(OFF);
  }
  Services &= ~RLY_SERVICE;		// reset the service flag
}
