/*
 * TempController.cpp
 *
 *  Created on: Sep 13, 2012
 *      Author: sfeltner
 */

#include "TempController.h"

TempController::TempController(const char *initName, uint8_t initIndex)
  : TempSensors(initName, initIndex)
{
	TemperatureActual = GetTemperature();
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

}

TempController::~TempController() {
	// TODO Auto-generated destructor stub
}

