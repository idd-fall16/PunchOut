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

int flag = 0; //keeps track so only first press is recorded
int flag1 = 0;

void setup(void) {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

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
  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("X:  "); Serial.print(lis.x); 
  Serial.print("  \tY:  "); Serial.print(lis.y); 
  Serial.print("  \tZ:  "); Serial.print(lis.z); 

  lis1.read();      // get X Y and Z data at once
  // Then print out the raw data
  //Serial.print("X1:  "); Serial.print(lis1.x); 
  //Serial.print("  \tY1:  "); Serial.print(lis1.y); 
  //Serial.print("  \tZ1:  "); Serial.print(lis1.z); 

  /* Or....get a new sensor event, normalized */ 
  sensors_event_t event; 
  lis.getEvent(&event);

  sensors_event_t event1; 
  lis1.getEvent(&event1);
  
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
  Serial.println(" m/s^2 ");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tX: "); Serial.print(event1.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event1.acceleration.y); 
  Serial.print(" \tZ: "); Serial.print(event1.acceleration.z); 
  Serial.println(" m/s^2 ");
  
  //detects which punch for accelerometer 1
  if(event.acceleration.y < -5) {
    if (flag == 0) {
      Serial.println("High");
      flag = 1;
    }
  }
  
  else if(event.acceleration.y > 5) {
    if (flag == 0) {
      Serial.println("Low");
      flag = 1;
    }
  }

  else {
    flag = 0;
  }
  
  //detects which punch for accelerometer 2
  if(event1.acceleration.y < -5) {
    if (flag1 == 0) {
      Serial.println("High 1");
      flag1 = 1;
    }
  }
  
  else if(event1.acceleration.y > 5) {
    if (flag1 == 0) {
      Serial.println("Low 1");
      flag1 = 1;
    }
  }

  else {
    flag1 = 0;
  }

  Serial.println();
 
  delay(300); 
}
