#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht11.h>
#include <OLEDFourBit.h>
#include <MsTimer2.h>
#include <Time.h>
#include "TCLExample.h"
#include "Clock.h"

//#define DEBUG
#include <DebugUtils.h>

dht11 DHT;
int  button1State = 0;
int  button2State = 0;
byte buttonSetting = 25;  // Default setting
byte Services      = 0;

byte DS_Interval   = 0;
byte DHT_Interval  = 0;
byte BTN_Interval  = 0;

unsigned long lastButton1Millis = 0;
byte lastButton1State = 0;
unsigned long lastButton2Millis = 0;
byte lastButton2State = 0;

// init the OLED
OLEDFourBit lcd(3, 4, 5, 6, 7, 8, 9);

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//celsius to fahrenheit conversion
float c2f(float val){
  float aux = (val * 9 / 5);
  return (aux + 32);
}

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}


void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // initialize the button pins as an input:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  // Init Relay module control pins - reverse logic, LOW = ON
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);

  // analog pins are input by default
  analogReference(2); //set analog reference to internal 1.1V
  delay(100);

  // Start up the DS18B20 library
  sensors.begin();

  // Set DS18B20 resolution to:
  //   9bit = 0.5C,    93.75ms time to convert (tCONV/8)
  //  10bit = 0.25C,  187.5ms  time to convert (tCONV/4)
  //  11bit = 0.125C  375ms    time to convert (tCONV/2)
  //  12bit = 0.0625C 750ms    time to convert
  sensors.setResolution(10);

  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity(%),\tTemp(C),\tTemp(F)");

  lcd.begin(20, 4);
  lcd.clear();
  lcd.home();

  lcd.setCursor(0,0);
  lcd.print("DHT-DS TEST PROGRAM ");

  lcd.display();		// Make sure the display is turned on!

  // Set interval counters
  DS_Interval  = DS_INTERVAL;
  DHT_Interval = DHT_INTERVAL;
  BTN_Interval = BTN_INTERVAL;

  // fire up the timer interrupt!
  MsTimer2::set(1000, timerISR);
  MsTimer2::start();
}

void timerISR(){
	Services |= CLK_SERVICE;			// service Clock once a second

    if (--DS_Interval <= 0){
    	Services |= DS_SERVICE;		// service DS18B20 once a second
        DS_Interval = DS_INTERVAL;	// reset service interval
    }
    if (--DHT_Interval <= 0){
    	Services |= DHT_SERVICE;		// service DHT11 once a second
    	DHT_Interval = DHT_INTERVAL;	// reset service interval
    }
	if (--BTN_Interval <= 0){
    	Services |= BTN_SERVICE;	// service DHT11 once a second
    	BTN_Interval = BTN_INTERVAL;	// reset service interval
    }
}

void loop(void)
{
  PROFILER1_START("Services Start");
  if (Services & CLK_SERVICE){
	  CLKControl();
  }
  if (Services & DS_SERVICE){
	  DSControl();
  }
  if (Services & DHT_SERVICE){
	  DHTControl();
  }
  if (Services & BTN_SERVICE){
	  buttonControl();
  }
  PROFILER1_END("Services End");

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
//  DEBUG_PRINT("button1 Start");
  PROFILER3_START("button1 Start");
  if((button1State = analogRead(button1)) > 900){
    if(lastButton1State == 0){        // is this the first time it was pressed?
	  Serial.println("button1 Pressed");
	  lastButton1State++;              // show the button is already pressed
    }
    else if ((lastButton1State) && ((millis() - lastButton1Millis) > BUTTON_BOUNCE)){
	  Serial.println("button1 Still Pressed - Incrementing Setting");
	  Serial.println(millis() - lastButton1Millis);
	  buttonSetting++;
    }
	lastButton1Millis = millis();      //   capture the millis for the next go around
  }
  else if (lastButton1State){
	lastButton1State = 0;              // otherwise the button was just released
	Serial.println("button1 Released");
  }
//  DEBUG_PRINT("button1 End");
  PROFILER3_END("button1 End");

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  PROFILER4_START("button2 Start");
//  DEBUG_PRINT("button2 Start");
  if((button2State = analogRead(button2)) > 900){
    if(lastButton2State == 0){        // is this the first time it was pressed?
	  Serial.println("button2 Pressed");
	  lastButton2State++;              // show the button is already pressed
    }
    else if ((lastButton2State) && ((millis() - lastButton2Millis) > BUTTON_BOUNCE)){
	  Serial.println("button2 Still Pressed - Decrementing Setting");
	  Serial.println(millis() - lastButton2Millis);
	  buttonSetting--;
    }
	lastButton2Millis = millis();      //   capture the millis for the next go around
  }
  else if (lastButton2State){
	lastButton2State = 0;              // otherwise the button was just released
	Serial.println("button2 Released");
  }
//  DEBUG_PRINT("button2 End");
  PROFILER4_END("button2 End");

//  DEBUG_PRINT("Setting Start");
  lcd.setCursor(0,1);
  lcd.print("Setting: ");
  lcd.print(buttonSetting);
//  DEBUG_PRINT("Setting End");

//  DEBUG_PRINT("Serial Start");
  if(Serial.available()){
    processSyncMessage();
  }
//  DEBUG_PRINT("Serial End");
}


void CLKControl(){
  if(timeStatus() != timeNotSet){  // here if the time has been set
    lcd.setCursor(12,1);
    OLEDClockDisplay();
  }
  Services &= ~CLK_SERVICE;		// reset the service flag
}

void DSControl(){
  float tempC;
  float tempF;

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
//  Serial.print("Requesting temperatures...");
  PROFILER2_START("DSControl:sensors.requestTemperatures()");
  sensors.requestTemperatures(); // Send the command to get temperatures
  PROFILER2_END("DSControl:sensors.requestTemperatures()");
//  Serial.println("DONE");

  lcd.setCursor(0,2);

  PROFILER5_START("DSControl:sensors.getTempCByIndex(0)");
  tempC = sensors.getTempCByIndex(0);
  PROFILER5_END("DSControl:sensors.getTempCByIndex(0)");
//  Serial.print("Temperature for the device 1 (index 0) is: ");
//  Serial.println(tempC);

////  tempF = c2f(tempC);

//  Serial.print("Temp = ");
//  Serial.print(tempC);
//  Serial.print(" C or ");
//  Serial.print(tempF);
//  Serial.println(" F");

  lcd.print("DS-OK");
  lcd.print("  ");
  lcd.print(tempC);
  lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111
////  lcd.print(" ");
////  lcd.print(tempF);
////  lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111

  Services &= ~DS_SERVICE;		// reset the service flag
}


void DHTControl(){
//  Serial.print("DHT11, \t");
  lcd.setCursor(0,3);
  int chk = DHT.read(DHT11_PIN);	// Read data from DHT11
  switch (chk){
    case DHTLIB_OK:
//		Serial.print("OK,\t");
		lcd.print("DHTOK ");
//		Serial.print(DHT.humidity,1);
		lcd.print(DHT.humidity);
		lcd.print('%');
//		Serial.print(",\t");
//		Serial.print(DHT.temperature,1);
		lcd.print(" ");
		lcd.print(DHT.temperature);
		lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111
//		Serial.print(",\t");
//		Serial.println(Fahrenheit(DHT.temperature),2);
  	    lcd.print(" ");
	    lcd.print(Fahrenheit(DHT.temperature));
		lcd.print((char)223);		// Print degree symbol 0xDF b1101 1111
		break;
    case DHTLIB_ERROR_CHECKSUM:
//		Serial.println("Checksum error,\t");
		lcd.print("Checksum error");
		break;
    case DHTLIB_ERROR_TIMEOUT:
//		Serial.println("Time out error,\t");
		lcd.print("Time out error");
		break;
    default:
//		Serial.println("Unknown error,\t");
		lcd.print("Unknown error");
		break;
  }
  Services &= ~DHT_SERVICE;		// reset the service flag
}

void buttonControl(){

//  Serial.println("Servicing Relay LEDs");
  if(buttonSetting > DHT.temperature){
	  digitalWrite(RELAY_1, LOW);
	  digitalWrite(RELAY_2, HIGH);
  }
  else if(buttonSetting < DHT.temperature){
	  digitalWrite(RELAY_1, HIGH);
	  digitalWrite(RELAY_2, LOW);
  }
  else{
	  digitalWrite(RELAY_1, LOW);
	  digitalWrite(RELAY_2, LOW);
  }
  Services &= ~BTN_SERVICE;		// reset the service flag
}
