/*
 * BeerTempController.cpp
 *
 *  Created on: Sep 13, 2012
 *      Author: sfeltner
 */

#include "BeerTempController.h"
#include "TCLExample.h"

BeerTempController::BeerTempController(const char* initName, uint8_t initIndex)
{
	_timer = 0;
	_Temp = new TempSensors("Beer",1);

	_Temp->_lastTempRequest = 0;

	TemperatureSetting = STARTUP_TEMP_DEFAULT;
//	Serial.println("BeerTempController");
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
//		updateSlowFilteredTemperatures();

	Slope = 0;
	TempHistoryIndex = 0;

	for (int i = 0; i < 30; i++) {
		TempHistory[i] = TempFiltSlow[3];
	}
}

BeerTempController::~BeerTempController() {
	// TODO Auto-generated destructor stub
}

//float BeerTempController::GetTemp()
//{
//	return _Temp->GetTemperature();
//}
//
float BeerTempController::SetTempSetting(float newSetting)
{
	return TemperatureSetting = newSetting;
}

float BeerTempController::GetTempActual()
{
	return TemperatureActual;
}

float BeerTempController::GetTempSetting()
{
	return TemperatureSetting;
}

float BeerTempController::GetThirdOrderTemp()
{
	return 0;
	//Run the algorithm
}

void BeerTempController::UpdateTimer()
{
	_timer+=200;

//	Serial.print("Beer Before ");
//	Serial.println(_timer);
	//Check the timers
	if ((_timer % 200) == 0){
//		Serial.println("Updating Beer Fast Temps");
		updateTemperatures();					// Update Fast Temperatures every 200ms
	}
	if((_timer % 10000) == 0){					// Update Slow Temperatures every 10 seconds
//		Serial.println("Updating Beer Slow Temps");
		updateSlowFilteredTemperatures();
		Serial.print(TempFast[3]);
		Serial.print(',');
		Serial.print(TempFiltFast[3]);
		Serial.print(',');
		Serial.print(TempFiltSlow[3]);
		Serial.print(',');
	}
	if((_timer % 60000) == 0){
//		Serial.println("Updating Beer Slope");
		updateSlope();
		_timer = 0;
	}
//	Serial.print("Beer After ");
//	Serial.println(_timer);
}

void BeerTempController::updateSlope()  //called every minute
{
	TempHistory[TempHistoryIndex] = TempFiltSlow[3];
	Slope = TempHistory[TempHistoryIndex]
		- TempHistory[(TempHistoryIndex + 1) % 30];
	TempHistoryIndex = (TempHistoryIndex + 1) % 30;
	Services &= ~updateSlope_SERVICE;		// reset the service flag
}
void BeerTempController::updateSlowFilteredTemperatures() //called every 10 seconds
{

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

	Services &= ~updateSlowFilteredTemperatures_SERVICE;		// reset the service flag
}


void BeerTempController::updateTemperatures() { //called every 200 milliseconds
//	Serial.println("Beer1");
//	Serial.println(TempFast[1]);
	TempFast[0] = TempFast[1];
//	Serial.println("Beer2");
//	Serial.println(TempFast[2]);
	TempFast[1] = TempFast[2];
//	Serial.println("Beer3");
//	Serial.println(TempFast[3]);
	TempFast[2] = TempFast[3];
	TempFast[3] = _Temp->GetTemperature();
//	Serial.println("Beer4");
//	Serial.println(TempFast[3]);
//	if (TempFast[2] != TempFast[3]){
//		Serial.print("BeerActual: ");
//		Serial.print(TempFast[2],10);
//		Serial.print(" ");
//		Serial.println(TempFast[3],10);
//	}

	// Butterworth filter with cutoff frequency 0.01*sample frequency (FS=5Hz)
//	Serial.println("Beer5");
//	Serial.println(TempFiltFast[1]);
	TempFiltFast[0] = TempFiltFast[1];
//	Serial.println("Beer6");
//	Serial.println(TempFiltFast[2]);
	TempFiltFast[1] = TempFiltFast[2];
//	Serial.println("Beer7");
//	Serial.println(TempFiltFast[3]);
	TempFiltFast[2] = TempFiltFast[3];
//	Serial.println("Beer8");
//	Serial.println(TempFast[0]);
	TempFiltFast[3] = (TempFast[0] + TempFast[3]
		+ 3 * (TempFast[1] + TempFast[2])) / 3.430944333e+04
		+ (0.8818931306 * TempFiltFast[0])
		+ (-2.7564831952 * TempFiltFast[1])
		+ (2.8743568927 * TempFiltFast[2]);
//	Serial.println("Beer9");
//	Serial.println(TempFiltFast[3]);
	TemperatureActual = TempFiltFast[3];

	Services &= ~updateTemperatures_SERVICE;		// reset the service flag
}

void BeerTempController::SerialPrintActualTemp()
{
	char myName[20];
	_Temp->GetName(myName);
	Serial.print(myName);
	Serial.print(" = ");
	Serial.print(TemperatureActual);
	Serial.println((char)248);	// ASCII 248 = degree symbol
}

void BeerTempController::LcdPrintActualTemp(OLEDFourBit *lcd)
{
	char myName[20];
	_Temp->GetName(myName);
	lcd->print(myName);
	lcd->print(" ");
	lcd->print(TemperatureActual);
	lcd->print((char)223);		// Print degree symbol 0xDF b1101 1111
}

