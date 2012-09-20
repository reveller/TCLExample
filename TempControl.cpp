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
