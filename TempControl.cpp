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
		lastIdleTime=millis();
		if(((timeSinceCooling() > 900000UL || doNegPeakDetect==0) && (timeSinceHeating()>600000UL || doPosPeakDetect==0)) || _state==STARTUP){ //if cooling is 15 min ago and heating 10
			if(fridgeTemperatureActual> fridgeTemperatureSetting+IDLE_RANGE_HIGH){
				if(mode!=FRIDGE_CONSTANT){
					if(beerTempFiltSlow[3]>beerTemperatureSetting+0.5){ // only start cooling when beer is too warm (0.05 degree idle space)
						_state=COOLING;
					}
				}
				else{
					_state=COOLING;
				}
				return;
			}
			if(fridgeTemperatureActual< fridgeTemperatureSetting+IDLE_RANGE_LOW){
				if(mode!=FRIDGE_CONSTANT){
					if(beerTempFiltSlow[3]<beerTemperatureSetting-0.5){ // only start heating when beer is too cold (0.05 degree idle space)
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
			doNegPeakDetect=0;  //peak would be from drifting in idle, not from cooling
		}
		if(timeSinceHeating()>900000UL){ //20 minutes
			doPosPeakDetect=0;  //peak would be from drifting in idle, not from heating
		}
		break;
	case COOLING:
		doNegPeakDetect=1;
		lastCoolTime = millis();
		estimatedOvershoot = coolOvershootEstimator  * min(MAX_COOL_TIME_FOR_ESTIMATE, (float) timeSinceIdle()/(1000))/60;
		estimatedPeakTemperature = fridgeTemperatureActual - estimatedOvershoot;
		if(estimatedPeakTemperature <= fridgeTemperatureSetting + COOLING_TARGET){
			fridgeSettingForNegPeakEstimate=fridgeTemperatureSetting;
			_state=IDLE;
			return;
		}
		break;
	case HEATING:
		lastHeatTime=millis();
		doPosPeakDetect=1;
		estimatedOvershoot = heatOvershootEstimator * min(MAX_HEAT_TIME_FOR_ESTIMATE, (float) timeSinceIdle()/(1000))/60;
		estimatedPeakTemperature = fridgeTemperatureActual + estimatedOvershoot;
		if(estimatedPeakTemperature >= fridgeTemperatureSetting + HEATING_TARGET){
			fridgeSettingForPosPeakEstimate=fridgeTemperatureSetting;
			_state=IDLE;
			return;
		}
		break;
	case DOOR_OPEN:
		if(digitalRead(doorPin) == LOW){
			serialFridgeMessage(FRIDGE_DOOR_CLOSED);
			_state=IDLE;
			return;
		}
	default:
		_state = 0xFF; //go to unknown state, should never happen
	}
}

//Update the timers
void TempControl::UpdateTimers()
{
	_beerTemp->UpdateTimer();
	_fridgeTemp->UpdateTimer();
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

void updateOutputs(void){
  switch (state){
  case IDLE:
  case STARTUP:
    digitalWrite(coolingPin, LOW);
    digitalWrite(heatingPin, LOW);
    break;
  case COOLING:
    digitalWrite(coolingPin, HIGH);
    digitalWrite(heatingPin, LOW);
    break;
  case HEATING:
  case DOOR_OPEN:
    digitalWrite(coolingPin, LOW);
    digitalWrite(heatingPin, HIGH);
    break;
  default:
    digitalWrite(coolingPin, LOW);
    digitalWrite(heatingPin, LOW);
    break;
  }
}

void detectPeaks(void){
  //detect peaks in fridge temperature to tune overshoot estimators
  if(doPosPeakDetect && state!=HEATING){
    if(fridgeTempFiltSlow[3] <= fridgeTempFiltSlow[2] && fridgeTempFiltSlow[2] >= fridgeTempFiltSlow[1]){ // maximum
      posPeak=fridgeTempFiltSlow[2];
      if(posPeak>fridgeSettingForPosPeakEstimate+HEATING_TARGET_UPPER){
        //should not happen, estimated overshoot was too low, so adjust overshoot estimator
        heatOvershootEstimator=heatOvershootEstimator*(1.2+min((posPeak-(fridgeSettingForPosPeakEstimate+HEATING_TARGET_UPPER))*.03,0.3));
        saveSettings();
      }
      if(posPeak<fridgeSettingForPosPeakEstimate+HEATING_TARGET_LOWER){
        //should not happen, estimated overshoot was too high, so adjust overshoot estimator
        heatOvershootEstimator=heatOvershootEstimator*(0.8+max((posPeak-(fridgeSettingForPosPeakEstimate+HEATING_TARGET_LOWER))*.03,-0.3));
        saveSettings();
      }
      doPosPeakDetect=0;
      serialFridgeMessage(POSPEAK);
    }
    else if(timeSinceHeating() > 580000UL && timeSinceCooling() > 900000UL && fridgeTempFiltSlow[3] < fridgeSettingForPosPeakEstimate+HEATING_TARGET_LOWER){
      //there was no peak, but the estimator is too low. This is the heat, then drift up situation.
        posPeak=fridgeTempFiltSlow[3];
        heatOvershootEstimator=heatOvershootEstimator*(0.8+max((posPeak-(fridgeSettingForPosPeakEstimate+HEATING_TARGET_LOWER))*.03,-0.3));
        saveSettings();
        doPosPeakDetect=0;
        serialFridgeMessage(POSDRIFT);
    }
  }
  if(doNegPeakDetect && state!=COOLING){
    if(fridgeTempFiltSlow[3] >= fridgeTempFiltSlow[2] && fridgeTempFiltSlow[2] <= fridgeTempFiltSlow[1]){ // minimum
      negPeak=fridgeTempFiltSlow[2];
      if(negPeak<fridgeSettingForNegPeakEstimate+COOLING_TARGET_LOWER){
        //should not happen, estimated overshoot was too low, so adjust overshoot estimator
        coolOvershootEstimator=coolOvershootEstimator*(1.2+min(((fridgeSettingForNegPeakEstimate+COOLING_TARGET_LOWER)-negPeak)*.03,0.3));
        saveSettings();
      }
      if(negPeak>fridgeSettingForNegPeakEstimate+COOLING_TARGET_UPPER){
        //should not happen, estimated overshoot was too high, so adjust overshoot estimator
        coolOvershootEstimator=coolOvershootEstimator*(0.8+max(((fridgeSettingForNegPeakEstimate+COOLING_TARGET_UPPER)-negPeak)*.03,-0.3));
        saveSettings();
      }
      doNegPeakDetect=0;
      serialFridgeMessage(NEGPEAK);
    }
    else if(timeSinceCooling() > 1780000UL && timeSinceHeating() > 1800000UL && fridgeTempFiltSlow[3] > fridgeSettingForNegPeakEstimate+COOLING_TARGET_UPPER){
      //there was no peak, but the estimator is too low. This is the cool, then drift down situation.
        negPeak=fridgeTempFiltSlow[3];
        coolOvershootEstimator=coolOvershootEstimator*(0.8+max((negPeak-(fridgeSettingForNegPeakEstimate+COOLING_TARGET_UPPER))*.03,-0.3));
        saveSettings();
        doNegPeakDetect=0;
        serialFridgeMessage(NEGDRIFT);
    }
  }
}


unsigned long timeSinceCooling(void){
  unsigned long currentTime = millis();
  unsigned long timeSinceLastOn;
  if(currentTime>=lastCoolTime){
    timeSinceLastOn = currentTime - lastCoolTime;
  }
  else{
    // millis() overflow has occured
    timeSinceLastOn = (currentTime + 1440000) - (lastCoolTime +1440000); // add a day to both for calculation
  }
  return timeSinceLastOn;
}


unsigned long timeSinceHeating(void){
  unsigned long currentTime = millis();
  unsigned long timeSinceLastOn;
  if(currentTime>=lastHeatTime){
    timeSinceLastOn = currentTime - lastHeatTime;
  }
  else{
    // millis() overflow has occured
    timeSinceLastOn = (currentTime + 1440000) - (lastHeatTime +1440000); // add a day to both for calculation
  }
  return timeSinceLastOn;
}


unsigned long timeSinceIdle(void){
  unsigned long currentTime = millis();
  unsigned long timeSinceLastOn;
  if(currentTime>=lastIdleTime){
    timeSinceLastOn = currentTime - lastIdleTime;
  }
  else{
    // millis() overflow has occured
    timeSinceLastOn = (currentTime + 1440000) - (lastIdleTime +1440000); // add a day to both for calculation
  }
  return timeSinceLastOn;
}

void initControl(void){
   if(beerTemperatureSetting<beerTempFiltSlow[3]){
     Kp=KpCool;
     Kd=KdCool;
   }
   else{
     Kp=KpHeat;
     Kd=KdHeat;
   }
}

