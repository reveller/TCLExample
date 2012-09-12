/*
 * TempSensors.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: sfeltner
 */

#include "TempSensors.h"

TempSensors::TempSensors() {
	// Setup a oneWire instance to communicate with any OneWire devices
	// (not just Maxim/Dallas temperature ICs)
	_onewire = new OneWire(ONE_WIRE_BUS);

	// Pass our oneWire reference to Dallas Temperature.
    _tempSensor = new DallasTemperature(_onewire);

	// Start up the DS18B20 library
	_tempSensor->begin();

	// Set DS18B20 resolution to:
	//   9bit = 0.5C,    93.75ms time to convert (tCONV/8)
	//  10bit = 0.25C,  187.5ms  time to convert (tCONV/4)
	//  11bit = 0.125C  375ms    time to convert (tCONV/2)
	//  12bit = 0.0625C 750ms    time to convert
//	_tempSensor->setResolution(10);
	_lastTempRequest = 0;
}

TempSensors::~TempSensors() {
	// TODO Auto-generated destructor stub
}

float TempSensors::GetTemperature(byte index)
{
	requestTemp();
	return _tempSensor->requestTemperaturesByIndex(index);
}

void TempSensors::requestTemp()
{
	int newMs = millis();
	if((newMs -_lastTempRequest)<MAX_TEMP_REQUEST)
		return;
	_tempSensor->requestTemperatures();
	_lastTempRequest = newMs;
}
