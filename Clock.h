/*
 * Clock.h
 *
 *  Created on: Sep 5, 2012
 *      Author: sfeltner
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#define TIME_MSG_LEN  11   // time sync consists of a HEADER followed by ten
                           // ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message

extern void SerialClockDisplay();
extern void SerialPrintDigits(int digits);
extern void OLEDClockDisplay();
extern void OLEDPrintDigits(int digits);
extern void processSyncMessage();



#endif /* CLOCK_H_ */
