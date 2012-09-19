/*
 * FridgeTempController.h
 *
 *  Created on: Sep 17, 2012
 *      Author: sfeltner
 */

#include "TempSensors.h"

#ifndef FRIDGETEMPCONTROLLER_H_
#define FRIDGETEMPCONTROLLER_H_

#define updateTemperatures_INTERVAL (1*CLOCK_RESOLUTION_MS)
#define updateSlowFilteredTemperatures_INTERVAL (10*CLOCK_RESOLUTION_MS)
#define updateSlope_INTERVAL (60*CLOCK_RESOLUTION_MS)


class FridgeTempController {
public:
	FridgeTempController();
	virtual ~FridgeTempController();
	float GetTemp();
	void UpdateTimer();
	void Update();
	float GetThirdOrderTemp();
private:
	TempSensors *_Temp;
	int _timer;
	byte _flags;
	void updateSlope();
	void updateSlowFilteredTemperatures();
	void updateTemperatures()
};

#endif /* FRIDGETEMPCONTROLLER_H_ */
