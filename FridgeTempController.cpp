/*
 * FridgeTempController.cpp
 *
 *  Created on: Sep 17, 2012
 *      Author: sfeltner
 */

#include "FridgeTempController.h"
#include "TCLExample.h"

FridgeTempController::FridgeTempController() {
	_Temp = new TempSensors("Fridge",0);

	_Temp->_lastTempRequest = 0;

	TemperatureActual = _Temp->GetTemperature();
	for (int i = 0; i < 4; i++) {
		TempFast[i] = TemperatureActual;
		TempFiltFast[i] = TemperatureActual;
	}
	for (int i = 0; i < 100; i++) {
//		updateTemperatures();
	}
	for (int i = 0; i < 4; i++) {
		TempSlow[i] = TempFiltFast[3];
		TempFiltSlow[i] = TempFiltFast[3];
	}
	for (int i = 0; i < 100; i++) {
//		updateSlowFilteredTemperatures();
	}

	Slope = 0;
	TempHistoryIndex = 0;

	for (int i = 0; i < 30; i++) {
		TempHistory[i] = TempFiltSlow[3];
	}
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

void FridgeTempController::updateSlope()  //called every minute
{
	TempHistory[TempHistoryIndex] = TempFiltSlow[3];
	Slope = TempHistory[TempHistoryIndex]
		- TempHistory[(TempHistoryIndex + 1) % 30];
	TempHistoryIndex = (TempHistoryIndex + 1) % 30;
	Services &= ~updateSlope_SERVICE;		// reset the service flag
}

void FridgeTempController::updateSlowFilteredTemperatures() //called every 10 seconds
{
	// Input for filter
	TempSlow[0] = TempSlow[1];
	TempSlow[1] = TempSlow[2];
	TempSlow[2] = TempSlow[3];
	TempSlow[3] = TempFiltFast[3];

	// Butterworth filter with cutoff frequency 0.01*sample frequency (FS=0.1Hz)
	TempFiltSlow[0] = TempFiltSlow[1];
	TempFiltSlow[1] = TempFiltSlow[2];
	TempFiltSlow[2] = TempFiltSlow[3];
	TempFiltSlow[3] = (TempSlow[0] + TempSlow[3]
	    + 3 * (TempSlow[1] + TempSlow[2])) / 3.430944333e+04
        + (0.8818931306 * TempFiltSlow[0])
        + (-2.7564831952 * TempFiltSlow[1])
	    + (2.8743568927 * TempFiltSlow[2]);

}
void FridgeTempController::updateTemperatures()  //called every 200 milliseconds
{
	TempFast[0] = TempFast[1];
	TempFast[1] = TempFast[2];
	TempFast[2] = TempFast[3];
	TempFast[3] = _Temp->GetTemperature();

	// Butterworth filter with cutoff frequency 0.033*sample frequency (FS=5Hz)
	TempFiltFast[0] = TempFiltFast[1];
	TempFiltFast[1] = TempFiltFast[2];
	TempFiltFast[2] = TempFiltFast[3];
	TempFiltFast[3] = (TempFast[0] + TempFast[3]
		+ 3 * (TempFast[1] + TempFast[2])) / 1.092799972e+03
		+ (0.6600489526 * TempFiltFast[0])
		+ (-2.2533982563 * TempFiltFast[1])
		+ (2.5860286592 * TempFiltFast[2]);

	TemperatureActual = TempFiltFast[3];
}
