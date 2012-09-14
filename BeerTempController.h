/*
 * BeerTempController.h
 *
 *  Created on: Sep 13, 2012
 *      Author: sfeltner
 */

#ifndef BEERTEMPCONTROLLER_H_
#define BEERTEMPCONTROLLER_H_

#include "TempController.h"

class BeerTempController : public TempController {
public:
	BeerTempController(const char *, uint8_t);
	virtual ~BeerTempController();

	float Slope;

	//history for slope calculation
	float TempHistory[30];
	unsigned char TempHistoryIndex;

};

#endif /* BEERTEMPCONTROLLER_H_ */
