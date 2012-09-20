/*
 * TempSensors.h
 *
 *  Created on: Sep 10, 2012
 *      Author: sfeltner
 */

#ifndef TEMPSENSORS_H_
#define TEMPSENSORS_H_

#include <OneWire.h>
#include <DallasTemperature.h>
#include <OLEDFourBit.h>

// Data wire is plugged into port 12 on the Arduino
#define ONE_WIRE_BUS 12
#define MAX_TEMP_REQUEST 900



class TempSensors{
public:
	TempSensors(const char *, uint8_t);
	~TempSensors();
	int     GetAddress(char *);
	float   GetTemperature();
	int     GetName(char *);
	void    LcdPrintTemp(OLEDFourBit *);
	void    SerialPrintTemp();
	int         _lastTempRequest;

	float   CurrentTemp;
	char    Name[20];
	uint8_t SensorIndex;
private:
	uint8_t            _resolution;
	OneWire           *_onewire;
	DallasTemperature *_sensors;
	uint8_t            _sensorAddr[8];

	void requestTemp();
};

#endif /* TEMPSENSORS_H_ */
