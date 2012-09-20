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

	lcd.begin(20, 4);
	lcd.clear();
	lcd.home();

	lcd.setCursor(0,0);
	lcd.print("DHT-DS TEST PROGRAM ");

	lcd.display();		// Make sure the display is turned on!

	// fire up the timer interrupt!
	MsTimer2::set(CLOCK_RESOLUTION_MS, timerISR);
	MsTimer2::start();
}

void timerISR(){
	Services |= CLK_SERVICE;			// service Clock once a second
	tempControl->UpdateTimers();
}

void loop(void)
{
	if (Services & CLK_SERVICE){
		CLKControl();
	}

	tempControl->AdjustTemp();

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

