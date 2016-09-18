/* Graph I2C Accelerometer On RedBear Duo over Serial Port
 * Adafruit Part 2809 LIS3DH - http://adafru.it/2809
 * This example shows how to program I2C manually
 * I2C Pins SDA1==D0, SCL1 == D1
 * Default address: 0x18
 */
 
// do not use the cloud functions - assume programming through Arduino IDE
#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL); 
#endif
// Basic demo for accelerometer readings from Adafruit LIS3DH

#include "Adafruit_LIS3DH.h"
#include "Adafruit_Sensor.h"

// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

//Use flags to keep track of only registering when it's the first press
int leftAccFlag = 0; //left accelerometer
int rightAccFlag = 0; //right accelerometer
int leftFlexFlag = 0; //left flex sensor
int rightFlexFlag = 0; //right flex sensor

//Sets pins for flex sensors
int flexSensorLeft = A0;
int flexSensorRight = A1;

void setup(void) {
  
  Serial.begin(9600);
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  
  //Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  //Serial.println("G");

  pinMode(flexSensorLeft, INPUT);    // sets pin as input - left flex sensor
  pinMode(flexSensorRight, INPUT);   // sets pin as input - right flex sensor
}

void loop() {
  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("X:  "); Serial.print(lis.x); 
  Serial.print("  \tY:  "); Serial.print(lis.y); 
  Serial.print("  \tZ:  "); Serial.print(lis.z); 

  /* Or....get a new sensor event, normalized */ 
  sensors_event_t event; 
  lis.getEvent(&event);
  
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
  Serial.println(" m/s^2 ");

    // Read in values of flex sensors
  /*
  int flexPositionLeft;    // Input value from the analog pin for left sensor
  int flexPositionRight;   // Input value from the analog pin for right sensor

  flexPositionLeft = analogRead(flexSensorLeft);
  flexPositionRight = analogRead(flexSensorRight);
  */

  //For debugging
  //Serial.print("Left flex sensor reading: ");
  //Serial.println(flexPositionLeft);
  //Serial.print("Right flex sensor reading: ");
  //Serial.println(flexPositionRight);
  
  //detects which punches have been thrown

  //defend - both go up at same time
  /*
  if(event.acceleration.y < -5 && event1.acceleration.y < -5) {
    if (leftAccFlag == 0 && rightAccFlag == 0) {
      Serial.println("Defend");
      leftAccFlag = 1;
      rightAccFlag = 1;
    }
  }

  //duck - both go down at same time
  else if(event.acceleration.y > 5 && event1.acceleration.y > 5) {
    if (leftAccFlag == 0 && rightAccFlag == 0) {
      Serial.println("Duck");
      leftAccFlag = 1;
      rightAccFlag = 1;
    }
  }
  */

  //left jab - left goes high
  if(event.acceleration.y < -5) {
    if (leftAccFlag == 0) {
      Serial.println("Left Jab");
      leftAccFlag = 1;
    }
  }

/*
  //right jab - right goes high
  else if(event1.acceleration.y < -5) {
    if (rightAccFlag == 0) {
      Serial.println("Right Jab");
      rightAccFlag = 1;
    }
  }
  */

  //left body - left goes low
  else if(event.acceleration.y > 5) {
    if (leftAccFlag == 0) {
      Serial.println("Left Body");
      leftAccFlag = 1;
    }
  }

/*
  //right body - right goes low
  else if(event1.acceleration.y > 5) {
    if (rightAccFlag == 0) {
      Serial.println("Right Body");
      rightAccFlag = 1;
    }
  }
  */

/*
  else if( flexPositionLeft < 3200) {
    if (leftFlexFlag == 0) {
      Serial.println("Dodge Left");
      leftFlexFlag = 1;
    }
  }

  else if( flexPositionRight < 3300) {
    if (rightFlexFlag == 0) {
      Serial.println("Dodge Right");
      rightFlexFlag = 1;
    }
  }
  */

  else {
    leftAccFlag = 0;
    rightAccFlag = 0;
    leftFlexFlag = 0;
    rightFlexFlag = 0;
  }

  Serial.println();
 
  delay(200); 
}


