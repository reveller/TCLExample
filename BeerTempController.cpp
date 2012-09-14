/*
 * BeerTempController.cpp
 *
 *  Created on: Sep 13, 2012
 *      Author: sfeltner
 */

#include "BeerTempController.h"

BeerTempController::BeerTempController(const char *initName, uint8_t initIndex)
  : TempController(initName, initIndex)
{
	Slope = 0;
	TempHistoryIndex = 0;

	for (int i = 0; i < 30; i++) {
		TempHistory[i] = TempController::TempFiltSlow[3];
	}
}

BeerTempController::~BeerTempController() {
	// TODO Auto-generated destructor stub
}

