/*
 * Display.h
 *
 *  Created on: Oct 1, 2012
 *      Author: sfeltner
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <OLEDFourBit.h>

class Display {
public:
	Display();
	virtual ~Display();
	void Draw(void);
	void SetMode(const char*);
	void SetState(const char*);
	void SetTime(long);
	float AtmosphereTemp;
	float LiquidTemp;
	long EpochTime;
	float TargetTemp;
private:
	float _preAtmosphereTemp;
	float _preLiquidTemp;
	long _preEpochTime;
	float _preTargetTemp;
	char* _mode;
	char* _state;
	bool _modeChange;
	bool _stateChange;
	OLEDFourBit* _lcd;
};

#endif /* DISPLAY_H_ */
