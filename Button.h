/*
 * Button.h
 *
 *  Created on: Sep 10, 2012
 *      Author: sfeltner
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <arduino.h>



#define BUTTON_BOUNCE 250 	// millis to wait to inc setting on continuous button press

class Button {
public:
	Button(uint8_t);
	~Button();
	bool IsPressed();
private:
	enum STATES {OPEN, CLOSED};
	unsigned long _lastMillis;
	bool _lastState;
	uint8_t _pin;
};

#endif /* BUTTON_H_ */
