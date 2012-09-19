/*
 * TempController.h
 *
 *  Created on: Sep 13, 2012
 *      Author: sfeltner
 */

#ifndef TEMPCONTROLLER_H_
#define TEMPCONTROLLER_H_

#include "TempSensors.h"

class TempController : public TempSensors {
public:
	TempController(const char *, uint8_t);
	virtual ~TempController();

	//
	TempController *fridgeTempController;

	// Fast filtered temperatures
	float TemperatureActual;
	float TemperatureSetting;

	float TempFast[4];
	float TempFiltFast[4];

	// Slow filtered Temperatures used for peak detection
	float TempSlow[4];
	float TempFiltSlow[4];

	float Slope;

	//history for slope calculation
	float TempHistory[30];
	unsigned char TempHistoryIndex;






};

#endif /* TEMPCONTROLLER_H_ */
