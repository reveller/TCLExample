/*
 * FridgeTempController.cpp
 *
 *  Created on: Sep 17, 2012
 *      Author: sfeltner
 */

#include "FridgeTempController.h"

FridgeTempController::FridgeTempController() {
	// TODO Auto-generated constructor stub
	_Temp = new TempSensors("Fridge",0);
}

FridgeTempController::~FridgeTempController() {
	// TODO Auto-generated destructor stub
}

float FridgeTempController::GetTemp()
{
	return _Temp->GetTemperature();
}

void FridgeTempController::Update()
{
	//Update Temp Array 200ms
	//Update Temp Slope Slow Filtered 10sec
	//Update Temp Slope 60secs

	if (_flags & updateTemperatures_SERVICE){
		Serial.println("updateTemperatures");
		updateTemperatures();
		_flags &= ~updateTemperatures_SERVICE;
	}
	if (_flags & updateSlowFilteredTemperatures_SERVICE){
		Serial.println("updateSlowFilteredTemperatures");
		updateSlowFilteredTemperatures();
		_flags &= ~updateSlowFilteredTemperatures_SERVICE;
	}
}

float FridgeTempController::GetThirdOrderTemp()
{
	//Run the algorithm
	return 0;
}

void FridgeTempController::UpdateTimer()
{
	_timer+=200;

	//Check the timers
	if(_timer%1000==0)
		_flags |= updateTemperatures_SERVICE;
	if(_timer%10000==0)
		_flags |= updateSlowFilteredTemperatures_SERVICE;
	if(_timer%60000==0)
		_flags |= updateSlope_SERVICE;
	if(_timer>=60000)
		_flags = 0;

}
void FridgeTempController::updateSlowFilteredTemperatures()
{

}
void FridgeTempController::updateTemperatures()
{

}
