/*
 * Settings.h
 *
 *  Created on: Sep 20, 2012
 *      Author: sfeltner
 */

#ifndef Settings_H_
#define Settings_H_

#include <EEPROM.h>

#define EEPROM_MODE 0
#define EEPROM_BEER_SETTING 1
#define EEPROM_FRIDGE_SETTING 3
#define EEPROM_HEAT_ESTIMATOR 5
#define EEPROM_COOL_ESTIMATOR 9



class Settings : public EEPROMClass{
public:
	Settings();
	virtual ~Settings();
	void Save(void);
	void Load(void);

private:
	void _writeFloat(int, float);
	float _readFloat(int);
	void _writeInt(int, int);
	int _readInt(int);
};

#endif /* Settings_H_ */
