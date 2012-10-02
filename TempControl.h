/*
 * TempControl.h
 *
 *  Created on: Sep 17, 2012
 *      Author: sfeltner
 */

#ifndef TEMPCONTROL_H_
#define TEMPCONTROL_H_

#include "BeerTempController.h"
#include "FridgeTempController.h"
#include "Relay.h"
#include "Settings.h"

// control defines
#define KpHeat 10
#define KpCool 5
#define Ki 0.02
#define KdCool -5
#define KdHeat -10

// Stay Idle when temperature is in this range
#define IDLE_RANGE_HIGH (+3)
#define IDLE_RANGE_LOW (-3)

// when peak falls between these limits, its good.
#define HEATING_TARGET_UPPER (+2)
#define HEATING_TARGET_LOWER (-1)
#define COOLING_TARGET_UPPER (+1)
#define COOLING_TARGET_LOWER (-2)

#define COOLING_TARGET ((COOLING_TARGET_UPPER+COOLING_TARGET_LOWER)/2)
#define HEATING_TARGET ((HEATING_TARGET_UPPER+HEATING_TARGET_LOWER)/2)

// maximum history to take into account, in seconds
#define MAX_HEAT_TIME_FOR_ESTIMATE 600
#define MAX_COOL_TIME_FOR_ESTIMATE 1200

class TempControl {
public:
	TempControl();
	virtual ~TempControl();
	void AdjustTemp();
	void UpdateTimers();

	void UpdatePIDSettings(void);
	void InitializePIDControl(void);



	float GetBeerTemp();
	float GetFridgeTemp();

	void LcdPrintBeerTemp(OLEDFourBit *);
	void LcdPrintFridgeTemp(OLEDFourBit *);

	float GetCurrentTempSetting();
	void SetCurrentTempSetting(char);

	enum modes_t{
	  FRIDGE_CONSTANT,
	  BEER_CONSTANT,
	  BEER_PROFILE,
	  modes_FIRST = FRIDGE_CONSTANT,
	  modes_LAST = BEER_PROFILE
	};

	enum states_t{
	  UNKNOWN,
	  COOLING,
	  HEATING,
	  IDLE,
	  STARTUP,
	  DOOR_OPEN,
	  states_FIRST = UNKNOWN,
	  states_LAST = DOOR_OPEN
	};

	modes_t GetMode();
	void SetMode(modes_t);
	const char* GetModeStr(void);
	const char* GetStateStr(void);

private:
	BeerTempController *_beerTemp;
	FridgeTempController *_fridgeTemp;
	Relay *_Compressor;
	Relay *_Heater;
	Settings *_settings;

	int _primaryController;
	unsigned long _timer;
	states_t _state;
	modes_t _mode;
	float _Kp;
	float _Kd;
	float _Ki;
	unsigned long _lastCoolTime;
	unsigned long _lastHeatTime;
	unsigned long _lastIdleTime;
	void UpdateTimer();
	float differenceIntegral;

	bool  _doNegPeakDetect;
	bool  _doPosPeakDetect;
	float _posPeak;
	float _negPeak;

	float _SettingForNegPeakEstimate;
	float _SettingForPosPeakEstimate;

	void detectPeaks(void);
	float _heatOvershootEstimator;
	float _coolOvershootEstimator;

	unsigned long timeSinceCooling(void);
	unsigned long timeSinceHeating(void);
	unsigned long timeSinceIdle(void);

	void UpdateState();
	void UpdateOutputs(void);
	void UpdateDisplay();
	void _SetNegPeakEstimate();
	void _SetPosPeakEstimate();
	float _GetSettingForPosPeakEstimate();
	float _GetSettingForNegPeakEstimate();

	void SerialPrintFridgeTemp();

	void SerialPrintBeerTemp();

	char _currentModeString[10];
	char _currentStateString[10];
};

#endif /* TEMPCONTROL_H_ */
