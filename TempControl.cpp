/*
 * TempControl.cpp
 *
 *  Created on: Sep 17, 2012
 *      Author: sfeltner
 */

#include "TempControl.h"
#include "OLEDFourBit.h"

TempControl::TempControl() {
	// TODO Auto-generated constructor stub
	_Compressor = new Relay(0,true);
	_Heater     = new Relay(1,true);
	_beerTemp   = new BeerTempController("Beer", 1);
	_fridgeTemp = new FridgeTempController();
	_settings   = new Settings();
	_timer=0;
}

TempControl::~TempControl() {
	// TODO Auto-generated destructor stub
}

void TempControl::AdjustTemp()
{
	//Run through the updates

//	if(buttonSetting > fridgeTempController->CurrentTemp){
//		relay1.SetState(ON);
//		relay2.SetState(OFF);
//	}
//	else if(buttonSetting < fridgeTempController->CurrentTemp){
//		relay1.SetState(OFF);
//		relay2.SetState(ON);
//	}
//	else{
//		relay1.SetState(OFF);
//		relay2.SetState(OFF);
//	}



}

void TempControl::UpdateState()
{
	float estimatedOvershoot;
	float estimatedPeakTemperature;
	//update state
	//	if(digitalRead(doorPin) == HIGH){
	//		if(state!=DOOR_OPEN){
	//			serialFridgeMessage(FRIDGE_DOOR_OPEN);
	//		}
	//		state=DOOR_OPEN;
	//		return;
	//	}
	switch(_state){
	case STARTUP:
	case IDLE:
		_lastIdleTime=millis();
		if(((timeSinceCooling() > 900000UL || _doNegPeakDetect==0) &&
				(timeSinceHeating()>600000UL || _doPosPeakDetect==0)) ||
				_state==STARTUP){ //if cooling is 15 min ago and heating 10
			if(_fridgeTemp->GetTempActual()> _fridgeTemp->GetTempSetting() + IDLE_RANGE_HIGH){
				if (_mode!=FRIDGE_CONSTANT){
					if(_beerTemp->TempFiltSlow[3] > _beerTemp->GetTempSetting() + 0.5){ // only start cooling when beer is too warm (0.05 degree idle space)
						_state=COOLING;
					}
				}
				else{
					_state=COOLING;
				}
				return;
			}
			if (_fridgeTemp->GetTempActual() < _fridgeTemp->GetTempSetting() + IDLE_RANGE_LOW){
				if (_mode != FRIDGE_CONSTANT){
					if(_beerTemp->TempFiltSlow[3] < _beerTemp->GetTempSetting() - 0.5){ // only start heating when beer is too cold (0.05 degree idle space)
						_state=HEATING;
					}
				}
				else{
					_state=HEATING;
				}
				return;
			}
		}
		if(timeSinceCooling()>1800000UL){ //30 minutes
			_doNegPeakDetect=0;  //peak would be from drifting in idle, not from cooling
		}
		if(timeSinceHeating()>900000UL){ //20 minutes
			_doPosPeakDetect=0;  //peak would be from drifting in idle, not from heating
		}
		break;
	case COOLING:
		_doNegPeakDetect=1;
		_lastCoolTime = millis();
		estimatedOvershoot = _coolOvershootEstimator  * min(MAX_COOL_TIME_FOR_ESTIMATE, (float) timeSinceIdle()/(1000))/60;
		estimatedPeakTemperature = _fridgeTemp->GetTempActual() - estimatedOvershoot;
		if(estimatedPeakTemperature <= _fridgeTemp->GetTempSetting() + COOLING_TARGET){
			_fridgeTemp->SetNegPeakEstimate();
			_state=IDLE;
			return;
		}
		break;
	case HEATING:
		_lastHeatTime=millis();
		_doPosPeakDetect=1;
		estimatedOvershoot = _heatOvershootEstimator * min(MAX_HEAT_TIME_FOR_ESTIMATE, (float) timeSinceIdle()/(1000))/60;
		estimatedPeakTemperature = _fridgeTemp->GetTempActual() + estimatedOvershoot;
		if(estimatedPeakTemperature >= _fridgeTemp->GetTempSetting() + HEATING_TARGET){
			_fridgeTemp->SetPosPeakEstimate();
			_state=IDLE;
			return;
		}
		break;
	case DOOR_OPEN:
		//		if(digitalRead(doorPin) == LOW){
		//			serialFridgeMessage(FRIDGE_DOOR_CLOSED);
		//			_state=IDLE;
		return;
	case UNKNOWN:
	default:
		_state = UNKNOWN; //go to unknown state, should never happen
	}
}

//Update the timers
void TempControl::UpdateTimers()
{
	_beerTemp->UpdateTimer();
	_fridgeTemp->UpdateTimer();
	UpdatePeakDetectionTimer();
}

void TempControl::UpdatePeakDetectionTimer()
{
	_timer+=200;

	//Check the timers
	if(_timer%10000==0)
		detectPeaks();
	if(_timer>=60000)
		_Flags = 0;

}
void TempControl::SetPrimaryTemp(int TempControllerId)
{
	// _primaryController = SetPrimaryTemp;
}

float TempControl::GetBeerTemp()
{
	return _beerTemp->GetTemp();
}

float TempControl::GetFridgeTemp()
{
	return _fridgeTemp->GetTemp();
}

TempControl::modes_t TempControl::GetMode(){
	return _mode;
}

void TempControl::SerialPrintFridgeTemp()
{
	return _fridgeTemp->SerialPrintTemp();
}

void TempControl::LcdPrintFridgeTemp(OLEDFourBit *lcd)
{
	return _fridgeTemp->LcdPrintTemp(lcd);
}

void TempControl::SerialPrintBeerTemp()
{
	return _beerTemp->SerialPrintTemp();
}

void TempControl::LcdPrintBeerTemp(OLEDFourBit *lcd)
{
	return _beerTemp->LcdPrintTemp(lcd);
}

void TempControl::updateOutputs(void){
  switch (_state){
  case IDLE:
  case STARTUP:
//    digitalWrite(coolingPin, LOW);
//    digitalWrite(heatingPin, LOW);
    break;
  case COOLING:
//    digitalWrite(coolingPin, HIGH);
//    digitalWrite(heatingPin, LOW);
    break;
  case HEATING:
//  case DOOR_OPEN:
//    digitalWrite(coolingPin, LOW);
//    digitalWrite(heatingPin, HIGH);
    break;
  default:
//    digitalWrite(coolingPin, LOW);
//    digitalWrite(heatingPin, LOW);
    break;
  }
}

void TempControl::detectPeaks(void){
  //detect peaks in fridge temperature to tune overshoot estimators
  if(_doPosPeakDetect &&_state!=HEATING){
    if(_fridgeTemp->TempFiltSlow[3] <=_fridgeTemp->TempFiltSlow[2] &&_fridgeTemp->TempFiltSlow[2] >=_fridgeTemp->TempFiltSlow[1]){ // maximum
      _posPeak = _fridgeTemp->TempFiltSlow[2];
      if(_posPeak > _fridgeTemp->GetSettingForPosPeakEstimate() + HEATING_TARGET_UPPER){
        //should not happen, estimated overshoot was too low, so adjust overshoot estimator
        _heatOvershootEstimator=_heatOvershootEstimator*(1.2+min((_posPeak-(_fridgeTemp->GetSettingForPosPeakEstimate() + HEATING_TARGET_UPPER))*.03,0.3));
        _settings->Save();
      }
      if(_posPeak < _fridgeTemp->GetSettingForPosPeakEstimate() + HEATING_TARGET_LOWER){
        //should not happen, estimated overshoot was too high, so adjust overshoot estimator
        _heatOvershootEstimator=_heatOvershootEstimator*(0.8+max((_posPeak-(_fridgeTemp->GetSettingForPosPeakEstimate() + HEATING_TARGET_LOWER))*.03,-0.3));
        _settings->Save();
      }
      _doPosPeakDetect=0;
      // serialFridgeMessage(POSPEAK);
    }
    else if(timeSinceHeating() > 580000UL && timeSinceCooling() > 900000UL &&_fridgeTemp->TempFiltSlow[3] < _fridgeTemp->GetSettingForPosPeakEstimate() + HEATING_TARGET_LOWER){
      //there was no peak, but the estimator is too low. This is the heat, then drift up situation.
        _posPeak = _fridgeTemp->TempFiltSlow[3];
        _heatOvershootEstimator=_heatOvershootEstimator*(0.8+max((_posPeak-(_fridgeTemp->GetSettingForPosPeakEstimate() + HEATING_TARGET_LOWER))*.03,-0.3));
        _settings->Save();
        _doPosPeakDetect=0;
        // serialFridgeMessage(POSDRIFT);
    }
  }
  if(_doNegPeakDetect &&_state!=COOLING){
    if(_fridgeTemp->TempFiltSlow[3] >= _fridgeTemp->TempFiltSlow[2] &&_fridgeTemp->TempFiltSlow[2] <=_fridgeTemp->TempFiltSlow[1]){ // minimum
      _negPeak = _fridgeTemp->TempFiltSlow[2];
      if(_negPeak < _fridgeTemp->GetSettingForNegPeakEstimate() + COOLING_TARGET_LOWER){
        //should not happen, estimated overshoot was too low, so adjust overshoot estimator
        _coolOvershootEstimator=_coolOvershootEstimator*(1.2+min(((_fridgeTemp->GetSettingForNegPeakEstimate() + COOLING_TARGET_LOWER)-_negPeak)*.03,0.3));
        _settings->Save();
      }
      if(_negPeak > _fridgeTemp->GetSettingForNegPeakEstimate() + COOLING_TARGET_UPPER){
        //should not happen, estimated overshoot was too high, so adjust overshoot estimator
        _coolOvershootEstimator=_coolOvershootEstimator*(0.8+max(((_fridgeTemp->GetSettingForNegPeakEstimate() + COOLING_TARGET_UPPER)-_negPeak)*.03,-0.3));
        _settings->Save();
      }
      _doNegPeakDetect=0;
      // serialFridgeMessage(NEGPEAK);
    }
    else if(timeSinceCooling() > 1780000UL && timeSinceHeating() > 1800000UL &&_fridgeTemp->TempFiltSlow[3] > _fridgeTemp->GetSettingForNegPeakEstimate() + COOLING_TARGET_UPPER){
      //there was no peak, but the estimator is too low. This is the cool, then drift down situation.
        _negPeak = _fridgeTemp->TempFiltSlow[3];
        _coolOvershootEstimator=_coolOvershootEstimator*(0.8+max((_negPeak-(_fridgeTemp->GetSettingForNegPeakEstimate() + COOLING_TARGET_UPPER))*.03,-0.3));
        _settings->Save();
        _doNegPeakDetect=0;
        // serialFridgeMessage(NEGDRIFT);
    }
  }
}


unsigned long TempControl::timeSinceCooling(void){
  unsigned long currentTime = millis();
  unsigned long timeSinceLastOn;
  if(currentTime>=_lastCoolTime){
    timeSinceLastOn = currentTime - _lastCoolTime;
  }
  else{
    // millis() overflow has occured
    timeSinceLastOn = (currentTime + 1440000) - (_lastCoolTime +1440000); // add a day to both for calculation
  }
  return timeSinceLastOn;
}


unsigned long TempControl::timeSinceHeating(void){
  unsigned long currentTime = millis();
  unsigned long timeSinceLastOn;
  if(currentTime>=_lastHeatTime){
    timeSinceLastOn = currentTime - _lastHeatTime;
  }
  else{
    // millis() overflow has occured
    timeSinceLastOn = (currentTime + 1440000) - (_lastHeatTime +1440000); // add a day to both for calculation
  }
  return timeSinceLastOn;
}


unsigned long TempControl::timeSinceIdle(void){
  unsigned long currentTime = millis();
  unsigned long timeSinceLastOn;
  if(currentTime>=_lastIdleTime){
    timeSinceLastOn = currentTime - _lastIdleTime;
  }
  else{
    // millis() overflow has occured
    timeSinceLastOn = (currentTime + 1440000) - (_lastIdleTime +1440000); // add a day to both for calculation
  }
  return timeSinceLastOn;
}

void TempControl::ReinitializeControl(void){
   if(_beerTemp->GetTempSetting() < _beerTemp->TempFiltSlow[3]){
     _Kp=KpCool;
     _Kd=KdCool;
   }
   else{
     _Kp=KpHeat;
     _Kd=KdHeat;
   }
}

