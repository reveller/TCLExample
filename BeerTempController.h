/*
 * BeerTempController.h
 *
 *  Created on: Sep 13, 2012
 *      Author: sfeltner
 */
#include "TempSensors.h"

#ifndef BEERTEMPCONTROLLER_H_
#define BEERTEMPCONTROLLER_H_

#define updateTemperatures_INTERVAL (1*CLOCK_RESOLUTION_MS)
#define updateSlowFilteredTemperatures_INTERVAL (10*CLOCK_RESOLUTION_MS)
#define updateSlope_INTERVAL (60*CLOCK_RESOLUTION_MS)


class BeerTempController {
public:
	BeerTempController(const char*, uint8_t);
	virtual ~BeerTempController();
	float GetTemp();
	void UpdateTimer();
	void Update();
	float GetThirdOrderTemp();
private:
	TempSensors *_Temp;
	int _timer;
	byte _flags;

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

	void updateSlope();
	void updateSlowFilteredTemperatures();
	void updateTemperatures();
};


#endif /* BEERTEMPCONTROLLER_H_ */