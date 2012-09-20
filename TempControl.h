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

class TempControl {
public:
	TempControl();
	virtual ~TempControl();
	void AdjustTemp();
	void UpdateTimers();
	void SetPrimaryTemp(int);
	float GetBeerTemp();
	float GetFridgeTemp();
	void SerialPrintFridgeTemp();
	void LcdPrintFridgeTemp(OLEDFourBit *);
	void SerialPrintBeerTemp();
	void LcdPrintBeerTemp(OLEDFourBit *);

private:
	BeerTempController *_beerTemp;
	FridgeTempController *_fridgeTemp;
	Relay *_Compressor;
	Relay *_Heater;
	int _primaryController;
	int _timer;
	byte _Flags;
};

#endif /* TEMPCONTROL_H_ */
