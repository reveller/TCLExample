/*
 * TempSensors.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: sfeltner
 */

#include "TempSensors.h"

TempSensors::TempSensors(const char *initName, uint8_t initIndex, DallasTemperature *sensors) {
	// Setup a oneWire instance to communicate with any OneWire devices
	// (not just Maxim/Dallas temperature ICs)
//	_onewire = new OneWire(ONE_WIRE_BUS);

	// Pass our oneWire reference to Dallas Temperature.
//    _tempSensor = new DallasTemperature(_onewire);

	// Start up the DS18B20 library
//	_tempSensor->begin();

	// Set DS18B20 resolution to:
	//   9bit = 0.5C,    93.75ms time to convert (tCONV/8)
	//  10bit = 0.25C,  187.5ms  time to convert (tCONV/4)
	//  11bit = 0.125C  375ms    time to convert (tCONV/2)
	//  12bit = 0.0625C 750ms    time to convert
//	_tempSensor->setResolution(10);

	_lastTempRequest = 0;
	CurrentTemp = 0;
	strncpy(Name, initName, 20);
	SensorIndex = initIndex;
	_resolution = 10;
	_sensors = sensors;
	_sensors->getAddress(SensorAddr, SensorIndex);
	_sensors->setResolution(_resolution);
}

TempSensors::~TempSensors() {
	// TODO Auto-generated destructor stub
}

float TempSensors::GetTemperature()
{
	requestTemp();
	CurrentTemp = _sensors->getTempCByIndex(SensorIndex);
	return CurrentTemp;
}

void TempSensors::LcdPrintTemp(OLEDFourBit *lcd)
{
	  lcd->print(Name);
	  lcd->print(" ");
	  lcd->print(CurrentTemp);
	  lcd->print((char)223);		// Print degree symbol 0xDF b1101 1111
}

void TempSensors::SerialPrintTemp()
{
	  Serial.print(Name);
	  Serial.print(" = ");
	  Serial.print(CurrentTemp);
	  Serial.println((char)248);	// ASCII 248 = degree symbol
}

void TempSensors::requestTemp()
{
	int newMs = millis();
	if((newMs -_lastTempRequest)<MAX_TEMP_REQUEST)
		return;
	_sensors->requestTemperatures();
	_lastTempRequest = newMs;
}
