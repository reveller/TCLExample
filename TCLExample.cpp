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
#include "Display.h"
#include "Command.h"


//#define DEBUG
#include <DebugUtils.h>

//dht11 DHT;

#define BUTTONMENU_PIN A5
#define BUTTONDOWN_PIN A6  // 13
#define BUTTONUP_PIN   A7  // 10

//Button buttonMenu(BUTTONMENU_PIN);
//Button buttonUp(BUTTONUP_PIN);
//Button buttonDown(BUTTONDOWN_PIN);

byte buttonSetting = 25;  // Default setting

byte Services      = 0;

byte DS_Interval   = 0;
byte DHT_Interval  = 0;
byte RLY_Interval  = 0;
byte updateTemperatures_Interval = 0;
byte updateSlowFilteredTemperatures_Interval = 0;
byte updateSlope_Interval = 0;

TempControl *tempControl;
Display *display;
Command * command;

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
	command = new Command(tempControl);

	display = new Display();

	// fire up the timer interrupt!
//	MsTimer2::set(CLOCK_RESOLUTION_MS, timerISR);
//	MsTimer2::start();
}

void timerISR(){
	Services |= CLK_SERVICE;			// service Clock once a second
}

void loop(void)
{
	static TempControl::modes_t menuFlag = TempControl::FRIDGE_CONSTANT;
	byte menuTemp = 0;

//	if (buttonUp.IsPressed()){
//		Serial.println("UP Button Pressed ");
//		tempControl->SetCurrentTempSetting(1);
//	}
//
//	if (buttonDown.IsPressed()){
//		Serial.println("DOWN Button Pressed ");
//		tempControl->SetCurrentTempSetting(-1);
//	}
//
//	if (buttonMenu.IsPressed()){
//		menuTemp = (byte)menuFlag + 1;
//		if (menuTemp > TempControl::modes_LAST){
//			menuTemp = TempControl::modes_FIRST;
//		}
//		menuFlag = (TempControl::modes_t)menuTemp;
//		tempControl->SetMode(menuFlag);
//
//	}

	if (Services & CLK_SERVICE){
		tempControl->UpdateTimers();

		Services &= ~CLK_SERVICE;

		//redraw the LCD display
//		display->LiquidTemp = tempControl->GetBeerTemp();
//		display->AtmosphereTemp = tempControl->GetFridgeTemp();
//		display->TargetTemp = tempControl->GetCurrentTempSetting();
//		display->SetMode(tempControl->GetModeStr());
//		display->SetState(tempControl->GetStateStr());
//		display->Draw();
	}

	command->Execute();
}

