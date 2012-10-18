/*
 * TempSensors.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: sfeltner
 */

#include "TempSensors.h"
#include <string.h>

#define ONE_WIRE_BUS 12

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//Required to support the static member
int TempSensors::_lastTempRequest;

// TempSensors::TempSensors(const char *initName, uint8_t initIndex, DallasTemperature *sensors) {
TempSensors::TempSensors(const char *initName, uint8_t initIndex) {
	// Setup a oneWire instance to communicate with any OneWire devices
	// (not just Maxim/Dallas temperature ICs)
	_onewire = new OneWire(ONE_WIRE_BUS);

	// Pass our oneWire reference to Dallas Temperature.
    _sensors = new DallasTemperature(_onewire);

	// Start up the DS18B20 library
	_sensors->begin();

	// Set DS18B20 resolution to:
	//   9bit = 0.5C,    93.75ms time to convert (tCONV/8)
	//  10bit = 0.25C,  187.5ms  time to convert (tCONV/4)
	//  11bit = 0.125C  375ms    time to convert (tCONV/2)
	//  12bit = 0.0625C 750ms    time to convert
	_sensors->setResolution(10);



	CurrentTemp = 0;
	strncpy(Name, initName, sizeof(Name));
	SensorIndex = initIndex;
	_resolution = 9;
	_sensors->getAddress(_sensorAddr, SensorIndex);
	_sensors->setResolution(_resolution);
}

TempSensors::~TempSensors() {
	// TODO Auto-generated destructor stub
}

int TempSensors::GetAddress(char *addrBuffer)
{
//	for (int i = 0, addrIdx = 0; i < 8; i++) {
//		if(_sensorAddr[i] < 0xf){
//			if (addrIdx < addrBufLen)
//				addrBuffer[addrIdx++] = '0';
//		if (addrIdx < addrBufLen)
//			itoa(_sensorAddr[i], addrBuffer[addrIdx++], HEX);
//		if (addrIdx < addrBufLen)
//			addrBuffer[addrIdx++] = ':';
		return sprintf(addrBuffer, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
				_sensorAddr[0],
				_sensorAddr[1],
				_sensorAddr[2],
				_sensorAddr[3],
				_sensorAddr[4],
				_sensorAddr[5],
				_sensorAddr[6],
				_sensorAddr[7]);
}

float TempSensors::GetTemperature()
{
//	Serial.print("Requesting Temp for: ");
//	Serial.print(Name);
//	Serial.print(" ");
	requestTemp();
	CurrentTemp = _sensors->getTempCByIndex(SensorIndex) * 10;
//	Serial.println(CurrentTemp);
	return CurrentTemp;
}

int TempSensors::GetName(char *dest)
{
	return (sprintf(dest, "%s", Name));
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
	if((newMs - TempSensors::_lastTempRequest)<MAX_TEMP_REQUEST)
		return;
	_sensors->requestTemperatures();
	_lastTempRequest = newMs;
}
