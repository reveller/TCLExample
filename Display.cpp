/*
 * Display.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: sfeltner
 */

#include "Display.h"
#include <Arduino.h>
#include <OLEDFourBit.h>
#include <Time.h>

Display::Display() {
	_mode = NULL;
	_state = NULL;
	_lcd = new OLEDFourBit(3, 4, 5, 6, 7, 8, 9);
	_modeChange = true;
	_stateChange = true;
}

Display::~Display() {
	// TODO Auto-generated destructor stub
}

void Display::Draw(void)
{
	if(TargetTemp!=_preTargetTemp)
	{
		_lcd->setCursor(0,1);
		_lcd->print("Targ ");
		_lcd->print(TargetTemp);
		_preTargetTemp = TargetTemp;
	}


	if(AtmosphereTemp!=_preAtmosphereTemp){
		//Draw and save atmosphere
		_lcd->setCursor(0,2);
		_lcd->print("Atmo ");
		_lcd->print(AtmosphereTemp);
		_preAtmosphereTemp = AtmosphereTemp;
	}

	if(LiquidTemp != _preLiquidTemp){
		//Draw and save liquid temp
		_lcd->setCursor(0,3);
		_lcd->print("Beer ");
		_lcd->print(LiquidTemp);
		_preLiquidTemp = LiquidTemp;
	}

	if(_modeChange)	{
		_lcd->setCursor(12,2);
		_lcd->print(_mode);
		_modeChange = false;
	}

//	Serial.println(_state);
//	Serial.println(" Needs to Change? ");
//	Serial.println(_stateChange);
	if(_stateChange)	{
		_lcd->setCursor(12,3);
		_lcd->print(_state);
		_stateChange = false;
	}

	//Draw time update
	if(timeStatus() != timeNotSet){  // here if the time has been set
			_lcd->setCursor(12,1);
			_lcd->print(hour());
			_lcd->print(":");
			_lcd->print(minute());
			_lcd->print(":");
			_lcd->print(second());
	}
}
void Display::SetMode(const char* Mode)
{
	//check if my mode is null
	if(Mode==NULL)
		return;

	//check for first run
	if(_mode==NULL){
		int len = strlen(Mode);
		_mode = (char*)malloc(len);
		strcpy(_mode,Mode);
		return;
	}

	//Check for same mode
	if(strcmp(Mode,_mode)==0){
		return;
	}

	free(_mode);
	int newModeLen = strlen(Mode);
	_mode = (char*)malloc(newModeLen+1);
	strcpy(_mode,Mode);
	_modeChange = true;
}

void Display::SetState(const char* State)
{
//	Serial.print("THIS IS THE STATE");
//	Serial.println(State);
	//check if my mode is null
	if(State==NULL)
		return;

	//check for first run
	if(_state==NULL){
		int len = strlen(State);
		_state = (char*)malloc(len);
		strcpy(_state,State);
		_stateChange = true;
		return;
	}

	//Check for same mode
	if(strcmp(State,_state)==0){
		return;
	}

	free(_state);
	int newModeLen = strlen(State);
	_state = (char*)malloc(newModeLen+1);
	strcpy(_state,State);
	_stateChange = true;
	//Serial.print("New State ");
	//Serial.println(_stateChange);
}
