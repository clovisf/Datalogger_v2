#include <Wire.h> // wire library for the BMP085 pressure sensor
#include <SD.h> // SDcard library
#include <Adafruit_BMP085.h> // the very own adafruit BMP085 code
#include "DHT.h" // temperature/humidity sensor DHT11

#define DHTPIN A3 // pin in which the DHT11 is connected
//Below you get to choose which DHTxx you have in hands: 
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//-------------------

/*
Arduino UNO Pin-mapping:

3.3V -> Vcc for BMP085
5V -> Vcc for DHT11
GND -> ground for DHT11
GND -> ground for BMP085

A0 -> LM35 analog reading (temperature)
A1 -> Battery voltage novdatment
A2 -> - 
A3 -> DHT11 digital reading (OneWire, temperature and humidity)
A4 -> BMP085 digital readig (SDA)
A5 -> BMP085 digital readig (SCL)

*/

const int chipSelect = 8; // chip-select pin for the SDcard
float tempLM35 = A0;    // input pin for LM35 temperature sensor
float LM35celsius = 0;  // variable to store the value coming from the temperature sensor
float batteryLevel = A1; // Supply battery voltage!
float batteryVoltage = 0;  // variable to store the value coming from the temperature sensor
//int redLed = 13;      // select the pin for the LED
Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);
unsigned long time; //for time novdatment
unsigned long previousTime;
boolean enterFunction= true;

void setup() {
  // declare the ledPin as an OUTPUT (just in case you need a led in your project
  //pinMode(redLed, OUTPUT);
  
  pinMode(10, OUTPUT); // pin assignment for the SDcard shield
  pinMode(9, OUTPUT); // pin assignment for the SDcard shield
  
  //blindversion //blindversion Serial.begin(9600); // initialize serial
  if (!bmp.begin()) {
	//blindversion Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

  if (!SD.begin(chipSelect)) {
    //blindversion Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  //blindversion Serial.println("card initialized.");
  
  File dataFile = SD.open("novdat.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.println("Temperature LM35,Battery Voltage,Internal Temp,Temperature BMP085,Altitude,Pressure,Humidity DHT11,Temperature DHT11\n"); // Change this text according
    // to what you are measuring; it works as a header for the saved data! 
    dataFile.close();
    }  
  // if the file isn't open, pop up an error:
  else {
    //blindversion Serial.println("error opening novdat.csv");
  }
  
    dht.begin(); // initialize the DHTxx humidity/temperature senssor
}


void loop() {
    time = micros();
    
    if (enterFunction == true){
      previousTime= time;
      //digitalWrite(redLed, HIGH);
   
   // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    //blindversion Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  //blindversion Serial.print("Humidity DHT11: "); 
  //blindversion Serial.print(h);
  //blindversion Serial.println("%");
  //blindversion Serial.print("Temperature DHT11: "); 
  //blindversion Serial.print(t);
  //blindversion Serial.println("C ");
  ////blindversion Serial.print(f);
  ////blindversion Serial.println("F");
  ////blindversion Serial.print("Heat index DHT11: ");
  ////blindversion Serial.print(hi);
  ////blindversion Serial.println("F"); 
   
  LM35celsius = analogRead(tempLM35)*0.488; // LM35celsius (temperature)= tempLM35*5V*100C / 1024
  batteryVoltage = analogRead(batteryLevel)*0.00488; //
  
  //blindversion Serial.print("temperature LM35: ");
  //blindversion Serial.println(LM35celsius);
  //blindversion Serial.print("Battery Voltage: ");
  //blindversion Serial.println(batteryVoltage);
 
    ////blindversion Serial.println(time); // Elapsed time (since last iteration) - for development
    //blindversion Serial.print("Temperature BMP085= ");
    //blindversion Serial.print(bmp.readTemperature());
    //blindversion Serial.println(" C");    
    //blindversion Serial.print("Pressure BMP085= ");
    //blindversion Serial.print(bmp.readPressure());
    //blindversion Serial.println(" Pa");    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    //blindversion Serial.print("Altitude BMP085= ");
    //blindversion Serial.print(bmp.readAltitude());
    //blindversion Serial.println(" meters");
    //blindversion Serial.print("Pressure at sea level (calculated) BMP085= ");
    //blindversion Serial.print(bmp.readSealevelPressure());
    //blindversion Serial.println(" Pa");
    // you can get a more precise novdatment of altitude
    // if you know the current sea level pressure which will
    // vary with weather and such. If it is 1015 millibars
    // that is equal to 101500 Pascals.
    //blindversion Serial.print("Real altitude BMP085= ");
    //blindversion Serial.print(bmp.readAltitude(102900)); //from: http://vancouver.weatherstats.ca/
    //blindversion Serial.println(" meters");
    //blindversion Serial.println();
    
    //-------------------
    // The routine for writing data to SDcard: 
    File dataFile = SD.open("novdat.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.print(LM35celsius);
    dataFile.print(",");
    dataFile.print(batteryVoltage);
    dataFile.print(",");
    dataFile.print(GetTemp(),1);
    dataFile.print(",");    
    dataFile.print(bmp.readTemperature());
    dataFile.print(",");
    dataFile.print(bmp.readAltitude(102900));
    dataFile.print(",");
    dataFile.print(bmp.readPressure());
    dataFile.print(",");
    dataFile.print(h);
    dataFile.print(",");
    dataFile.print(t);
    dataFile.print("\n");
    
    dataFile.close();
    // print to the serial port too:
    //blindversion Serial.println(bmp.readTemperature());
    //blindversion Serial.println(GetTemp(),1);
  }  
  // if the file isn't open, pop up an error:
  else {
    //blindversion Serial.println("error opening novdat.csv");
  }
  // end of SDcard writing routine!
  //----------------------
    
    }
     
     //digitalWrite(redLed, LOW);
     if (time-previousTime < 4999990){ //executes all the routing every (roughly) 5 seconds
      enterFunction= false;
    } else {
      enterFunction= true;
    }
    }
    
    //------------------
    // End of main program 
    //------------------

    // For reading the internal temperature ->>
double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celcius.
  return (t);
}


