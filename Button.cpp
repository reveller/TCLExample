/*
 * Button.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: sfeltner
 */

#include "Button.h"

Button::Button(uint8_t pin) {
	_lastMillis = 0;
	_lastState = 0;
	_pin = pin;

	pinMode(_pin, INPUT);
}

Button::~Button() {
	// TODO Auto-generated destructor stub
}

bool Button::IsPressed(){
	int pinread;		// MUST BE int to do AnalogRead
	if((pinread = analogRead(_pin)) > 900){
	  if(_lastState == 0){        // is this the first time it was pressed?
		  _lastState++;              // show the button is already pressed
	  }
	  else if ((_lastState) && ((millis() - _lastMillis) > BUTTON_BOUNCE)){
          _lastMillis = millis();      //   capture the millis for the next go around
		  return true;
	  }
	}
	else if (_lastState){
		_lastState = 0;              // otherwise the button was just released
		_lastMillis = 0;
	}
	return false;
}
