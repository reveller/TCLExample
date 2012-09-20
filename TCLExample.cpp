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
#include "TempControl.h"
#include "Settings.h"


//#define DEBUG
#include <DebugUtils.h>

dht11 DHT;

#define BUTTONMENU_PIN A5
#define BUTTONDOWN_PIN A6  // 13
#define BUTTONUP_PIN   A7  // 10

Button buttonMenu(BUTTONMENU_PIN);
Button buttonUp(BUTTONUP_PIN);
Button buttonDown(BUTTONDOWN_PIN);

byte buttonSetting = 25;  // Default setting

byte Services      = 0;

byte DS_Interval   = 0;
byte DHT_Interval  = 0;
byte RLY_Interval  = 0;
byte updateTemperatures_Interval = 0;
byte updateSlowFilteredTemperatures_Interval = 0;
byte updateSlope_Interval = 0;

Relay relay1(RELAY_1, OFF);
Relay relay2(RELAY_2, OFF);

// init the OLED
OLEDFourBit lcd(3, 4, 5, 6, 7, 8, 9);

TempControl *tempControl;

Settings settings();

void setup(void)
{
	// start serial port
	Serial.begin(9600);

	// analog pins are input by default - for the buttons - move to class?
	analogReference(2); //set analog reference to internal 1.1V
	delay(100);

	Serial.println("BEER TEST PROGRAM ");
	Serial.println();

	tempControl = new TempControl();

//
//	// 01:34:67:90:23:56:89:01
//	char addrBuffer[24];
//	int retval = 0;
//	if((retval = fridgeTempController->GetAddress(addrBuffer)) == 23){
//		Serial.print("fridgeTempController Address:[");
//		Serial.print(addrBuffer);
//		Serial.println("]");
//	}
//	if((retval = beerTempController->GetAddress(addrBuffer)) == 23){
//		Serial.print("beerTempController Address:[");
//		Serial.print(addrBuffer);
//		Serial.println("]");
//	}

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

	updateTemperatures_Interval = updateTemperatures_INTERVAL;

	updateSlowFilteredTemperatures_Interval = updateSlowFilteredTemperatures_INTERVAL;
	updateSlope_Interval = updateSlope_INTERVAL;

	// fire up the timer interrupt!
	MsTimer2::set(CLOCK_RESOLUTION_MS, timerISR);
	MsTimer2::start();


//	Services = DS_SERVICE | DHT_SERVICE | DHT_SERVICE;
}

void timerISR(){
	Services |= CLK_SERVICE;			// service Clock once a second
	tempControl->UpdateTimers();

//	if (--DS_Interval <= 0){
//		Services |= DS_SERVICE;		// service DS18B20 once a second
//		DS_Interval = DS_INTERVAL;	// reset service interval
//	}
//	if (--DHT_Interval <= 0){
//		Services |= DHT_SERVICE;		// service DHT11 once a second
//		DHT_Interval = DHT_INTERVAL;	// reset service interval
//	}
//	if (--RLY_Interval <= 0){
//		Services |= RLY_SERVICE;	// service DHT11 once a second
//		RLY_Interval = RLY_INTERVAL;	// reset service interval
//	}
//	if (--updateTemperatures_Interval <= 0){
//		Services |= updateTemperatures_SERVICE;
//		updateTemperatures_Interval = updateTemperatures_INTERVAL;	// reset service interval
//	}
//	if (--updateSlowFilteredTemperatures_Interval <= 0){
//		Services |= updateSlowFilteredTemperatures_SERVICE;
//		updateSlowFilteredTemperatures_Interval = updateSlowFilteredTemperatures_INTERVAL;	// reset service interval
//	}
//	if (--updateSlope_Interval <= 0){
//		Services |= updateSlope_SERVICE;
//		updateSlope_Interval = updateSlope_INTERVAL;	// reset service interval
//	}
}

void loop(void)
{

	PROFILER1_START("Services Start");
	if (Services & CLK_SERVICE){
	//	Serial.println("CLKControl");
		CLKControl();
	}

	tempControl->AdjustTemp();

//	if (Services & DS_SERVICE){
//		Serial.println("DSControl");
//		DSControl();
//	}
	//  if (Services & DHT_SERVICE){
	//	  DHTControl();
	//  }
//	if (Services & RLY_SERVICE){
//		Serial.println("RLYControl");
//		RLYControl();
//	}
//	if (Services & updateTemperatures_SERVICE){
//		Serial.println("updateTemperatures");
//		updateTemperatures();
//	}
//	if (Services & updateSlowFilteredTemperatures_SERVICE){
//		Serial.println("updateSlowFilteredTemperatures");
//		updateSlowFilteredTemperatures();
//	}
//	if (Services & updateSlope_SERVICE){
//		Serial.println("updateSlope");
//		updateSlope();
//	}


	PROFILER1_END("Services End");

	if (buttonUp.IsPressed())
		buttonSetting++;

	if (buttonDown.IsPressed())
		buttonSetting--;

	static bool menuFlag = 0;
	if (buttonMenu.IsPressed()){
		lcd.setCursor(15,1);
		menuFlag = !menuFlag;
		if(menuFlag)
			lcd.print("MENU");
		else
			lcd.print("    ");
	}

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

	tempControl->SerialPrintFridgeTemp();
	lcd.setCursor(0,2);
	tempControl->LcdPrintFridgeTemp(&lcd);

	tempControl->SerialPrintBeerTemp();
	lcd.setCursor(0,3);
	tempControl->LcdPrintBeerTemp(&lcd);

	Services &= ~CLK_SERVICE;		// reset the service flag
}

//void DSControl(){
//	fridgeTempController->GetTemperature();
//	beerTempController->GetTemperature();
//
//	fridgeTempController->SerialPrintTemp();
//	lcd.setCursor(0,2);
//	fridgeTempController->LcdPrintTemp(&lcd);
//
//	beerTempController->SerialPrintTemp();
//	lcd.setCursor(0,3);
//	beerTempController->LcdPrintTemp(&lcd);
//
//	Services &= ~DS_SERVICE;		// reset the service flag
//}


//void DHTControl(){
//	//  Serial.print("DHT11, \t");
//	lcd.setCursor(0,3);
//	int chk = DHT.read(DHT11_PIN);	// Read data from DHT11
//	switch (chk){
//	case DHTLIB_OK:
//		//		Serial.print("OK,\t");
//		lcd.print("DHTOK ");
//		//		Serial.print(DHT.humidity,1);
//		lcd.print(DHT.humidity);
//		lcd.print('%');
//		//		Serial.print(",\t");
//		//		Serial.print(DHT.temperature,1);
//		lcd.print(" ");
//		lcd.print(DHT.temperature);
//		lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111
//		//		Serial.print(",\t");
//		//		Serial.println(Fahrenheit(DHT.temperature),2);
//		//  	    lcd.print(" ");
//		//	    lcd.print(Fahrenheit(DHT.temperature));
//		//		lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111
//		break;
//	case DHTLIB_ERROR_CHECKSUM:
//		//		Serial.println("Checksum error,\t");
//		lcd.print("Checksum error");
//		break;
//	case DHTLIB_ERROR_TIMEOUT:
//		//		Serial.println("Time out error,\t");
//		lcd.print("Time out error");
//		break;
//	default:
//		//		Serial.println("Unknown error,\t");
//		lcd.print("Unknown error");
//		break;
//	}
//	Services &= ~DHT_SERVICE;		// reset the service flag
//}

void RLYControl(){

//	if(buttonSetting > fridgeTempController->CurrentTemp){
//		relay1.SetState(ON);
//		relay2.SetState(OFF);
//	}
//	else if(buttonSetting < fridgeTempController->CurrentTemp){
//		relay1.SetState(OFF);
//		relay2.SetState(ON);
//	}
//	else{
//		relay1.SetState(OFF);
//		relay2.SetState(OFF);
//	}
//	Services &= ~RLY_SERVICE;		// reset the service flag
}



