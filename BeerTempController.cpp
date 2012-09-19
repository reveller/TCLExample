/*
 * BeerTempController.cpp
 *
 *  Created on: Sep 13, 2012
 *      Author: sfeltner
 */

#include "BeerTempController.h"

BeerTempController::BeerTempController()

{
	_Temp = new TempSensors("Beer",1);

//	Slope = 0;
//	TempHistoryIndex = 0;
//
//	for (int i = 0; i < 30; i++) {
//		TempHistory[i] = TempController::TempFiltSlow[3];
//	}
}

BeerTempController::~BeerTempController() {
	// TODO Auto-generated destructor stub
}

float BeerTempController::GetTemp()
{
	return _Temp->GetTemperature();
}

void BeerTempController::Update()
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
	if (_flags & updateSlope_SERVICE){
		Serial.println("updateSlope");
		updateSlope();
		_flags &= ~updateSlope_SERVICE;
	}

}


float BeerTempController::GetThirdOrderTemp()
{
	return 0;
	//Run the algorithm
}

void BeerTempController::UpdateTimer()
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

void BeerTempController::updateSlope()
{

}
void BeerTempController::updateSlowFilteredTemperatures()
{

}
void BeerTempController::updateTemperatures()
{

}
