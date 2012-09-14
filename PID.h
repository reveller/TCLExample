/*
 * PID.h
 *
 *  Created on: Sep 13, 2012
 *      Author: sfeltner
 */

#ifndef PID_H_
#define PID_H_

class PID {
public:
	PID();
	virtual ~PID();

	// Control parameters
	float heatOvershootEstimator;
	float coolOvershootEstimator;
	float fridgeSettingForNegPeakEstimate;
	float fridgeSettingForPosPeakEstimate;
	float negPeak;
	float posPeak;
	float differenceIntegral;

};

#endif /* PID_H_ */
