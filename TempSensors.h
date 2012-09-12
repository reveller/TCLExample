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


// enum SENSOR_NAME {fridgeSensor, beerSensor};
//#define fridgeSensor 0
//#define beerSensor   1

class TempSensors{
public:
	TempSensors(const char *, uint8_t, DallasTemperature *);
	~TempSensors();
	float   GetTemperature();
	void    LcdPrintTemp(OLEDFourBit *);
	void    SerialPrintTemp();

	float   CurrentTemp;
	char    Name[20];
	uint8_t SensorIndex;
	uint8_t SensorAddr[8];
private:
	 int                _lastTempRequest;
	 uint8_t            _resolution;
	 DallasTemperature *_sensors;
	 void requestTemp();
};

#endif /* TEMPSENSORS_H_ */
