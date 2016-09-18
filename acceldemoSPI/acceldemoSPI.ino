// Basic demo for accelerometer readings from Adafruit LIS3DH
#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL); 
#endif

//#include <Wire.h>
//#include <SPI.h>
#include "application.h"
//#include <spark_wiring_i2c.h>
#include "spark_wiring.h"
#include "spark_wiring_i2c.h"
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS3DH1.h>

/*
// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10
*/

// Used for software SPI
#define LIS3DH_CLK A3 //SCL
#define LIS3DH_MISO A4 //SDO
#define LIS3DH_MOSI A5 //SDA
// Used for hardware & software SPI
#define LIS3DH_CS A2 //CS

// For second accelerometer
#define LIS3DH1_CLK D4 //SCL
#define LIS3DH1_MISO D3 //SDO
#define LIS3DH1_MOSI D2 //SDA
// Used for hardware & software SPI
#define LIS3DH1_CS D5 //CS


// software SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
Adafruit_LIS3DH1 lis1 = Adafruit_LIS3DH1(LIS3DH1_CS);
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// I2C
//Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

//Use flags to keep track of only registering when it's the first press
int leftAccFlag = 0; //left accelerometer
int rightAccFlag = 0; //right accelerometer
int leftFlexFlag = 0; //left flex sensor
int rightFlexFlag = 0; //right flex sensor

//Sets pins for flex sensors
int flexSensorLeft = A0;
int flexSensorRight = A1;

void setup(void) {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  pinMode(flexSensorLeft, INPUT);    // sets pin as input - left flex sensor
  pinMode(flexSensorRight, INPUT);   // sets pin as input - right flex sensor

  Serial.begin(9600);
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");

  if (! lis1.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");

  //Sets the range depending on how you want to scale the readings
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  lis1.setRange(LIS3DH1_RANGE_4_G);   // 2, 4, 8 or 16 G!
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");

  Serial.print("Range 1 = "); Serial.print(2 << lis1.getRange());  
  Serial.println("G");
}

void loop() {

  bool print_raw = false;
  bool print_data = false;
  
  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  if (print_raw) {
    Serial.print("X:  "); Serial.print(lis.x);
    Serial.print("  \tY:  "); Serial.print(lis.y);
    Serial.print("  \tZ:  "); Serial.print(lis.z);
  }

  lis1.read();      // get X Y and Z data at once
  // Then print out the raw data
  if (print_raw) {
    Serial.print("X1:  "); Serial.print(lis1.x);
    Serial.print("  \tY1:  "); Serial.print(lis1.y);
    Serial.print("  \tZ1:  "); Serial.print(lis1.z);
  }

  // normalize raw data values 
  sensors_event_t event; 
  lis.getEvent(&event);

  sensors_event_t event1; 
  lis1.getEvent(&event1);
  
  // Display the results (acceleration is measured in m/s^2)
  if (print_data) {
    Serial.print("\t\tX_L: "); Serial.print(event.acceleration.x);
    Serial.print(" \tY_L: "); Serial.print(event.acceleration.y); 
    Serial.print(" \tZ_L: "); Serial.print(event.acceleration.z); 
    Serial.println(" m/s^2 ");  
  }

  // Display the results (acceleration is measured in m/s^2)
  if (print_data) {
    Serial.print("\t\tX_R: "); Serial.print(event1.acceleration.x);
    Serial.print(" \tY_R: "); Serial.print(event1.acceleration.y); 
    Serial.print(" \tZ_R: "); Serial.print(event1.acceleration.z); 
    Serial.println(" m/s^2 ");
  }
  
  // Read in values of flex sensors
  int flexPositionLeft;    // Input value from the analog pin for left sensor
  int flexPositionRight;   // Input value from the analog pin for right sensor

  flexPositionLeft = analogRead(flexSensorLeft);
  flexPositionRight = analogRead(flexSensorRight);

  //For debugging
  //Serial.print("Left flex sensor reading: ");
  //Serial.println(flexPositionLeft);
  //Serial.print("Right flex sensor reading: ");
  //Serial.println(flexPositionRight);
  
  //detects which punches have been thrown
  
  int key_presses = 3;
  
  //defend - both go up at same time
  //if(event.acceleration.y < -5 && event1.acceleration.y < -5) {
  if(event.acceleration.z > 8 && event1.acceleration.z > 8 && event.acceleration.x < 4 && event1.acceleration.x > -4) {
    //if (leftAccFlag == 0 && rightAccFlag == 0) {
      Serial.println("Defend");
      for (int count = 0; count < key_presses; count++) {
        Serial.println(2);
      }
      leftAccFlag = 1;
      rightAccFlag = 1;
    //}
  }

  //duck - both go down at same time
  //else if(event.acceleration.y > 5 && event1.acceleration.y > 5) {
  else if(event.acceleration.z < -3 && event1.acceleration.z < -3) {
    //if (leftAccFlag == 0 && rightAccFlag == 0) {
      Serial.println("Duck");
      for (int count = 0; count < (2*key_presses); count++) {
        Serial.println(2);
      }
      leftAccFlag = 1;
      rightAccFlag = 1;
    //}
  }
  
  //left body - left goes low
  //else if(event.acceleration.y > 5) {
  else if(event.acceleration.z > 6 && event.acceleration.x > 6) {
    //if (leftAccFlag == 0) {
      Serial.println("Left Body");
      for (int count = 0; count < key_presses; count++) {
        Serial.println(8);
      }
      leftAccFlag = 1;
    //}
  }

  //right body - right goes low
  //else if(event1.acceleration.y > 5) {
  else if(event1.acceleration.z > 6 && event1.acceleration.x < -6) {  
    //if (rightAccFlag == 0) {
      Serial.println("Right Body");
      for (int count = 0; count < key_presses; count++) {
        Serial.println(7);
      }
      rightAccFlag = 1;
    //}
  }

  else if( flexPositionLeft < 3000) {
    //if (leftFlexFlag == 0) {
      Serial.println("Dodge Left");
      for (int count = 0; count < key_presses; count++) {
        Serial.println(3);
      }
      leftFlexFlag = 1;
    //}
  }

  else if( flexPositionRight < 2900) {
    //if (rightFlexFlag == 0) {
      Serial.println("Dodge Right");
      for (int count = 0; count < key_presses; count++) {
        Serial.println(4);
      }
      rightFlexFlag = 1;
    //}
  }

  else {
    leftAccFlag = 0;
    rightAccFlag = 0;
    leftFlexFlag = 0;
    rightFlexFlag = 0;
    //Serial.println("Reset");
  }

  Serial.println();
 
  delay(100);
}

