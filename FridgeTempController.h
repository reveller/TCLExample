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
	float GetTempActual();
	float GetTempSetting();
	float SetTempSetting(float);
	void UpdateTimer();
	void Update();
	float GetThirdOrderTemp();
	void SerialPrintTemp();
	void LcdPrintTemp(OLEDFourBit *);
	void SetNegPeakEstimate();
	void SetPosPeakEstimate();
	float GetSettingForPosPeakEstimate();
	float GetSettingForNegPeakEstimate();


	// Fast filtered temperatures
	float TemperatureActual;
	float TemperatureSetting;

	float TempFast[4];
	float TempFiltFast[4];

	// Slow filtered Temperatures used for peak detection
	float TempSlow[4];
	float TempFiltSlow[4];
private:
	TempSensors *_Temp;
	int _timer;
	byte _flags;


	float Slope;

	//history for slope calculation
	float TempHistory[30];
	unsigned char TempHistoryIndex;

	void updateSlope();
	void updateSlowFilteredTemperatures();
	void updateTemperatures();

	float SettingForNegPeakEstimate;
	float SettingForPosPeakEstimate;

	enum messages_t{
	  FRIDGE_SETTING_FROM_FRIDGE,
	  FRIDGE_SETTING_FROM_SERIAL,
	  FRIDGE_DOOR_OPEN,
	  FRIDGE_DOOR_CLOSED,
	  POSPEAK,
	  NEGPEAK,
	  POSDRIFT,
	  NEGDRIFT,
	  ARDUINO_START
	};
};

#endif /* FRIDGETEMPCONTROLLER_H_ */
