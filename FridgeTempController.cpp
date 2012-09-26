/*
 * FridgeTempController.cpp
 *
 *  Created on: Sep 17, 2012
 *      Author: sfeltner
 */

#include "FridgeTempController.h"
#include "TCLExample.h"

FridgeTempController::FridgeTempController() {
	_timer = 0;
	_Temp = new TempSensors("Fridge",0);

	_Temp->_lastTempRequest = 0;

	TemperatureSetting = STARTUP_TEMP_DEFAULT;
	TemperatureActual = _Temp->GetTemperature();
	for (int i = 0; i < 4; i++) {
		TempFast[i] = TemperatureActual;
		TempFiltFast[i] = TemperatureActual;
	}
//		updateTemperatures();
	for (int i = 0; i < 4; i++) {
		TempSlow[i] = TempFiltFast[3];
		TempFiltSlow[i] = TempFiltFast[3];
	}
		updateSlowFilteredTemperatures();

	Slope = 0;
	TempHistoryIndex = 0;

	for (int i = 0; i < 30; i++) {
		TempHistory[i] = TempFiltSlow[3];
	}
}

FridgeTempController::~FridgeTempController() {
	// TODO Auto-generated destructor stub
}

//float FridgeTempController::GetTemp()
//{
//	return _Temp->GetTemperature();
//}
//
float FridgeTempController::GetTempActual()
{
	return TemperatureActual;
}

float FridgeTempController::GetTempSetting()
{
	return TemperatureSetting;
}

float FridgeTempController::SetTempSetting(float newSetting)
{
	return TemperatureSetting = newSetting;
}


float FridgeTempController::GetThirdOrderTemp()
{
	//Run the algorithm
	return 0;
}

void FridgeTempController::UpdateTimer()
{
	_timer+=200;

//	Serial.print("Fridge Before ");
//	Serial.println(_timer);
	//Check the timers
	if ((_timer % 200) == 0){
//		Serial.println("Updating Fridge Fast Temps");
		updateTemperatures();					// Update Fast Temperatures every 200ms
	}
	if((_timer % 10000) == 0){					// Update Slow Temperatures every 10 seconds
//		Serial.println("Updating Fridge Slow Temps");
		updateSlowFilteredTemperatures();
	}
	if((_timer % 60000) == 0){
//		Serial.println("Updating Fridge Slope");
		updateSlope();
		_timer = 0;
	}
//	Serial.print("Fridge After ");
//	Serial.println(_timer);
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
//	Serial.println("Input");
	// Input for filter
	TempSlow[0] = TempSlow[1];
	TempSlow[1] = TempSlow[2];
	TempSlow[2] = TempSlow[3];
	TempSlow[3] = TempFiltFast[3];
//	Serial.println("Butterworth");
	// Butterworth filter with cutoff frequency 0.01*sample frequency (FS=0.1Hz)
//	Serial.println(TempFiltSlow[1]);
	TempFiltSlow[0] = TempFiltSlow[1];
//	Serial.println(TempFiltSlow[2]);
	TempFiltSlow[1] = TempFiltSlow[2];
//	Serial.println(TempFiltSlow[3]);
	TempFiltSlow[2] = TempFiltSlow[3];
//	Serial.println(TempSlow[0] + TempSlow[3]
//	                                      + 3 * (TempSlow[1] + TempSlow[2])) / 3.430944333e+04
//	                                      + (0.8818931306 * TempFiltSlow[0])
//	                                      + (-2.7564831952 * TempFiltSlow[1])
//	                                      + (2.8743568927 * TempFiltSlow[2]);
	TempFiltSlow[3] = (TempSlow[0] + TempSlow[3]
	    + 3 * (TempSlow[1] + TempSlow[2])) / 3.430944333e+04
        + (0.8818931306 * TempFiltSlow[0])
        + (-2.7564831952 * TempFiltSlow[1])
	    + (2.8743568927 * TempFiltSlow[2]);
//	Serial.println("Done");
}

void FridgeTempController::updateTemperatures()  //called every 200 milliseconds
{
	TempFast[0] = TempFast[1];
	TempFast[1] = TempFast[2];
	TempFast[2] = TempFast[3];
//	Serial.println("FridgeTempController::updateTemperatures");
	TempFast[3] = _Temp->GetTemperature();
//	if (TempFast[2] != TempFast[3]){
//		Serial.print("FridgeActual: ");
//		Serial.print(TempFast[2],10);
//		Serial.print(" ");
//		Serial.println(TempFast[3],10);
//	}

	// Butterworth filter with cutoff frequency 0.033*sample frequency (FS=5Hz)
	TempFiltFast[0] = TempFiltFast[1];
	TempFiltFast[1] = TempFiltFast[2];
	TempFiltFast[2] = TempFiltFast[3];
	TempFiltFast[3] = (TempFast[0] + TempFast[3]
		+ 3 * (TempFast[1] + TempFast[2])) / 1.092799972e+03
		+ (0.6600489526 * TempFiltFast[0])
		+ (-2.2533982563 * TempFiltFast[1])
		+ (2.5860286592 * TempFiltFast[2]);

//	if (TemperatureActual != TempFiltFast[3]){
//		Serial.print("FridgeActual: ");
//		Serial.print(TemperatureActual,10);
//		Serial.print(" ");
//		Serial.println(TempFiltFast[3],10);
//	}
	TemperatureActual = TempFiltFast[3];
//	Serial.println("FridgeActual");
//	Serial.println(TemperatureActual);
}

void FridgeTempController::SerialPrintActualTemp()
{
	char myName[20];
	_Temp->GetName(myName);
	Serial.print(myName);
	Serial.print(" = ");
	Serial.print(TemperatureActual,2);
	Serial.println((char)248);	// ASCII 248 = degree symbol
}

void FridgeTempController::LcdPrintActualTemp(OLEDFourBit *lcd)
{
	char myName[20];
	_Temp->GetName(myName);
	lcd->print(myName);
	lcd->print(" ");
	lcd->print(TemperatureActual,2);
	lcd->print((char)223);		// Print degree symbol 0xDF b1101 1111
}

