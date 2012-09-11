/*
 * Clock.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: sfeltner
 */

/*
 * TimeSerial sketch
 * example code illustrating Time library set through serial port messages.
 *
 * Messages consist of the letter T followed by ten digit time
 * (as seconds since Jan 1 1970)
 * You can send the text on the next line using Serial Monitor to set the
 * clock to noon Jan 1 2011:
 * T1293883200
 *
 * A Processing example sketch to automatically send the messages is
 * included in the Time library download
 */

#include <Time.h>
#include "Clock.h"
#include "TCLExample.h"

void SerialClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  SerialPrintDigits(minute());
  SerialPrintDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void OLEDClockDisplay(){
  // digital clock display of the time
  lcd.print(hour());
  OLEDPrintDigits(minute());
  OLEDPrintDigits(second());
}

void SerialPrintDigits(int digits){
  // utility function for digital clock display: prints preceding colon
  // and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void OLEDPrintDigits(int digits){
  // utility function for digital clock display: prints preceding colon
  // and leading 0
  lcd.print(":");
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  // time message consists of a header and ten ascii digits
  while(Serial.available() >=  TIME_MSG_LEN ){
    char c = Serial.read() ;
    Serial.print(c);
    if( c == TIME_HEADER ) {
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){
        c = Serial.read();
        if( isDigit(c)) {
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number
        }
      }
      setTime(pctime);   // Sync clock to the time received on serial port
    }
  }
}



