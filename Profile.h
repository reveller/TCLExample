/*
 * Profile.h
 *
 *  Created on: Oct 1, 2012
 *      Author: sfeltner
 */

#ifndef PROFILE_H_
#define PROFILE_H_

class Profile {
public:
	Profile();
	virtual ~Profile();
	bool CheckNext();
private:
	unsigned long _startTime;
	float _targetTemp;
	Profile *next;
};

#endif /* PROFILE_H_ */
