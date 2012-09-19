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
//#include "BeerTempController.h"

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

// Our temp sensors, we'll allocate them in setup()
//TempSensors *fridgeSensor;
//TempSensors *beerSensor;
//TempController *beerTempController;
//TempController *fridgeTempController;

TempControl *tempControl;

void setup(void)
{
	// start serial port
	Serial.begin(9600);

	// analog pins are input by default - for the buttons - move to class?
	analogReference(2); //set analog reference to internal 1.1V
	delay(100);

	Serial.println("BEER TEST PROGRAM ");
	Serial.println();

	//  fridgeSensor = new TempSensors("Fridge", 0);
	//  beerSensor   = new TempSensors();

	tempControl = new tempControl();

//	fridgeTempController = new TempController ("Fridge", 0);
//	beerTempController = new TempController ("Beer", 1);
//
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

void updateTemperatures(void) { //called every 200 milliseconds
	fridgeTempController->TempFast[0] = fridgeTempController->TempFast[1];
	fridgeTempController->TempFast[1] = fridgeTempController->TempFast[2];
	fridgeTempController->TempFast[2] = fridgeTempController->TempFast[3];
	fridgeTempController->TempFast[3] = fridgeTempController->TempSensors::GetTemperature();

	// Butterworth filter with cutoff frequency 0.033*sample frequency (FS=5Hz)
	fridgeTempController->TempFiltFast[0] = fridgeTempController->TempFiltFast[1];
	fridgeTempController->TempFiltFast[1] = fridgeTempController->TempFiltFast[2];
	fridgeTempController->TempFiltFast[2] = fridgeTempController->TempFiltFast[3];
	fridgeTempController->TempFiltFast[3] = (fridgeTempController->TempFast[0] + fridgeTempController->TempFast[3]
	    + 3 * (fridgeTempController->TempFast[1] + fridgeTempController->TempFast[2])) / 1.092799972e+03
	    + (0.6600489526 * fridgeTempController->TempFiltFast[0])
	    + (-2.2533982563 * fridgeTempController->TempFiltFast[1])
	    + (2.5860286592 * fridgeTempController->TempFiltFast[2]);

	fridgeTempController->TemperatureActual = fridgeTempController->TempFiltFast[3];

	beerTempController->TempFast[0] = beerTempController->TempFast[1];
	beerTempController->TempFast[1] = beerTempController->TempFast[2];
	beerTempController->TempFast[2] = beerTempController->TempFast[3];
	beerTempController->TempFast[3] = beerTempController->TempSensors::GetTemperature();

	// Butterworth filter with cutoff frequency 0.01*sample frequency (FS=5Hz)
	beerTempController->TempFiltFast[0] = beerTempController->TempFiltFast[1];
	beerTempController->TempFiltFast[1] = beerTempController->TempFiltFast[2];
	beerTempController->TempFiltFast[2] = beerTempController->TempFiltFast[3];
	beerTempController->TempFiltFast[3] = (beerTempController->TempFast[0] + beerTempController->TempFast[3]
	    + 3 * (beerTempController->TempFast[1] + beerTempController->TempFast[2])) / 3.430944333e+04
	    + (0.8818931306 * beerTempController->TempFiltFast[0])
	    + (-2.7564831952 * beerTempController->TempFiltFast[1])
	    + (2.8743568927 * beerTempController->TempFiltFast[2]);

	beerTempController->TemperatureActual = beerTempController->TempFiltFast[3];

	Services &= ~updateTemperatures_SERVICE;		// reset the service flag
}

void updateSlowFilteredTemperatures(void) { //called every 10 seconds
	// Input for filter
	fridgeTempController->TempSlow[0] = fridgeTempController->TempSlow[1];
	fridgeTempController->TempSlow[1] = fridgeTempController->TempSlow[2];
	fridgeTempController->TempSlow[2] = fridgeTempController->TempSlow[3];
	fridgeTempController->TempSlow[3] = fridgeTempController->TempFiltFast[3];

	// Butterworth filter with cutoff frequency 0.01*sample frequency (FS=0.1Hz)
	fridgeTempController->TempFiltSlow[0] = fridgeTempController->TempFiltSlow[1];
	fridgeTempController->TempFiltSlow[1] = fridgeTempController->TempFiltSlow[2];
	fridgeTempController->TempFiltSlow[2] = fridgeTempController->TempFiltSlow[3];
	fridgeTempController->TempFiltSlow[3] = (fridgeTempController->TempSlow[0] + fridgeTempController->TempSlow[3]
	    + 3 * (fridgeTempController->TempSlow[1] + fridgeTempController->TempSlow[2])) / 3.430944333e+04
        + (0.8818931306 * fridgeTempController->TempFiltSlow[0])
        + (-2.7564831952 * fridgeTempController->TempFiltSlow[1])
	    + (2.8743568927 * fridgeTempController->TempFiltSlow[2]);

	beerTempController->TempSlow[0] = beerTempController->TempSlow[1];
	beerTempController->TempSlow[1] = beerTempController->TempSlow[2];
	beerTempController->TempSlow[2] = beerTempController->TempSlow[3];
	beerTempController->TempSlow[3] = beerTempController->TempFiltFast[3];

	// Butterworth filter with cutoff frequency 0.01*sample frequency (FS=0.1Hz)
	beerTempController->TempFiltSlow[0] = beerTempController->TempFiltSlow[1];
	beerTempController->TempFiltSlow[1] = beerTempController->TempFiltSlow[2];
	beerTempController->TempFiltSlow[2] = beerTempController->TempFiltSlow[3];
	beerTempController->TempFiltSlow[3] = (beerTempController->TempSlow[0] + beerTempController->TempSlow[3]
        + 3 * (beerTempController->TempSlow[1] + beerTempController->TempSlow[2])) / 3.430944333e+04
        + (0.8818931306 * beerTempController->TempFiltSlow[0])
        + (-2.7564831952 * beerTempController->TempFiltSlow[1])
        + (2.8743568927 * beerTempController->TempFiltSlow[2]);

	Services &= ~updateSlowFilteredTemperatures_SERVICE;		// reset the service flag
}

void updateSlope(void) { //called every minute
	beerTempController->TempHistory[beerTempController->TempHistoryIndex] = beerTempController->TempFiltSlow[3];
	beerTempController->Slope = beerTempController->TempHistory[beerTempController->TempHistoryIndex]
	    - beerTempController->TempHistory[(beerTempController->TempHistoryIndex + 1) % 30];
	beerTempController->TempHistoryIndex = (beerTempController->TempHistoryIndex + 1) % 30;
	Services &= ~updateSlope_SERVICE;		// reset the service flag
}
