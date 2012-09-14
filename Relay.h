/*
 * Relay.h
 *
 *  Created on: Sep 10, 2012
 *      Author: sfeltner
 */

#ifndef RELAY_H_
#define RELAY_H_

#include <arduino.h>

#define ON  0x1
#define OFF 0x0

class Relay {
public:
	Relay(uint8_t, bool);
	~Relay();
	bool SetState(bool);
	bool GetState();
private:
	uint8_t _pin;
	bool _state;
};

#endif /* RELAY_H_ */
