/*
 * Settings.cpp
 *
 *  Created on: Sep 20, 2012
 *      Author: sfeltner
 */


#include <EEPROMClass.h>
#include "Settings.h"

Settings::Settings() {
	// TODO Auto-generated constructor stub

}

Settings::~Settings() {
	// TODO Auto-generated destructor stub
}

int Settings::_readInt(int address){
   int value = 0x0000;
   value = value | (read(address) << 8);
   value = value | read(address+1);
   return value;
}

void Settings::_writeInt(int address, int value){
   write(address, (value >> 8) & 0xFF );
   write(address+1, value & 0xFF);
}

float Settings::_readFloat(int address){
   union u_tag {
     uint8_t b[4];
     float fval;
   } u;
   u.b[0] = read(address);
   u.b[1] = read(address+1);
   u.b[2] = read(address+2);
   u.b[3] = read(address+3);
   return u.fval;
}

void Settings::_writeFloat(int address, float value){
   union u_tag {
     uint8_t b[4];
     float fval;
   } u;
   u.fval=value;

   write(address  , u.b[0]);
   write(address+1, u.b[1]);
   write(address+2, u.b[2]);
   write(address+3, u.b[3]);
}

void Settings::Save(void){
	// TODO
  // write new settings to EEPROM to save be able to reload them after a reset
  // Check if settings in EEPROM are still  correct to keep writes minimal (only 100.000 cycles)
//  if(_TempControl->GetMode() != read(EEPROM_MODE)){
//    write(EEPROM_MODE,mode);
//  }
//  int beerTemperatureSettingInt = int(beerTemperatureSetting+.5);
//  if(beerTemperatureSettingInt != _readInt(EEPROM_BEER_SETTING)){
//    _writeInt(EEPROM_BEER_SETTING,beerTemperatureSettingInt);
//  }
//  int fridgeTemperatureSettingInt = int(fridgeTemperatureSetting+.5);
//  if(fridgeTemperatureSettingInt != _readInt(EEPROM_FRIDGE_SETTING)){
//    _writeInt(EEPROM_FRIDGE_SETTING,fridgeTemperatureSettingInt);
//  }
//  if(heatOvershootEstimator != _readFloat(EEPROM_HEAT_ESTIMATOR)){
//     _writeFloat(EEPROM_HEAT_ESTIMATOR, heatOvershootEstimator);
//  }
//  if(coolOvershootEstimator != _readFloat(EEPROM_COOL_ESTIMATOR)){
//     _writeFloat(EEPROM_COOL_ESTIMATOR, coolOvershootEstimator);
//  }
}

void Settings::Load(void){
//  mode = read(EEPROM_MODE);
//  if(mode > BEER_PROFILE){
//    //setting in EEPROM is invalid
//    mode = BEER_CONSTANT;
//  }
//  beerTemperatureSetting = _readInt(EEPROM_BEER_SETTING);
//  if(beerTemperatureSetting > 300 | beerTemperatureSetting <0){
//    //setting in EEPROM is invalid
//    beerTemperatureSetting = 200;
//  }
//  fridgeTemperatureSetting = _readInt(EEPROM_FRIDGE_SETTING);
//  if(fridgeTemperatureSetting > 300 | fridgeTemperatureSetting <0){
//    //setting in EEPROM is invalid
//    fridgeTemperatureSetting = 200;
//  }
//  heatOvershootEstimator = _readFloat(EEPROM_HEAT_ESTIMATOR);
//  if(heatOvershootEstimator < 0 || heatOvershootEstimator > 1000 || heatOvershootEstimator==0x0000){ //incorrect value in EEPROM
//     heatOvershootEstimator=0.2;
//  }
//  coolOvershootEstimator = _readFloat(EEPROM_COOL_ESTIMATOR);
//  if(coolOvershootEstimator < 0 || coolOvershootEstimator > 1000 || coolOvershootEstimator==0x0000){ //incorrect value in EEPROM{
//    coolOvershootEstimator=5;
//  }
}
