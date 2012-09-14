/*
 * Relay.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: sfeltner
 */

#include "Relay.h"

Relay::Relay(uint8_t pin, bool startState) {

	_pin = pin;
	_state = startState;

	pinMode(_pin, OUTPUT);
	if(startState)
		digitalWrite(_pin, LOW);
	else
		digitalWrite(_pin, HIGH);

}

Relay::~Relay() {
	// TODO Auto-generated destructor stub
}

//Returns false if it does NOT perform a state change
bool Relay::SetState(bool newState){
	if(_state == newState)
		return false;
	else if(newState){
		_state = true;
		digitalWrite(_pin, LOW);
		return true;
	}
	else if(!newState){
		_state = false;
		digitalWrite(_pin, HIGH);
		return true;
	}
	return false;
}

//Returns false if it does NOT perform a state change
bool Relay::GetState(){
	return _state;
}

