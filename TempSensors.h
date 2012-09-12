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

// Data wire is plugged into port 12 on the Arduino
#define ONE_WIRE_BUS 12
#define MAX_TEMP_REQUEST 900


// enum SENSOR_NAME {fridgeSensor, beerSensor};
#define fridgeSensor 0
#define beerSensor   1

class TempSensors{
public:
	TempSensors();
	~TempSensors();
	float GetTemperature(byte);
private:
	 DallasTemperature *_tempSensor;
	 OneWire *_onewire;
	 int _lastTempRequest;
	 void requestTemp();
};

#endif /* TEMPSENSORS_H_ */
