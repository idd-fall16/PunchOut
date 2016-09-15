#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL); 
#endif

#include "application.h"
//#include <spark_wiring_i2c.h>
#include "spark_wiring.h"
#include "spark_wiring_i2c.h"

// LIS3DHTR I2C address is 18(24)
#define Addr 0x18

int xAccl = 0;
int yAccl = 0; 
int zAccl = 0;
void setup() 
{
    // Set variable
    //Particle.variable("i2cdevice","LIS3DHTR");
    //Particle.variable("xAccl", xAccl);
    //Particle.variable("yAccl", yAccl);
    //Particle.variable("zAccl", zAccl);

    // Optional set I2C clock speed
    Wire.setSpeed(400000);
  
    // Initialise I2C communication as MASTER
    Wire.begin();
    // Initialize serial communication, set baud rate = 9600
    Serial.begin(9600);
  
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select control register 1
    Wire.write(0x20);
    // Enable X, Y, Z axis, power on mode, data rate selection = 10Hz
    Wire.write(0x27);
    // Stop I2C Transmission
    Wire.endTransmission();
  
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select control register 4
    Wire.write(0x23);
    // Set continuos update, full scale range +/- 2g
    Wire.write(0x00);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}

void loop()
{
    unsigned int data[6];
    for(int i = 0; i < 6; i++)
    {
        // Start I2C Transmission
        Wire.beginTransmission(Addr);
        // Select data register
        Wire.write((40+i));
        // Stop I2C Transmission
        Wire.endTransmission();
        
        // Request 1 byte of data
        Wire.requestFrom(Addr, 1);
        
        // Read 6 bytes of data
        // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
        if(Wire.available() == 1)
        {
            data[i] = Wire.read();
        }
        delay(300);
    }
    
    // Convert the data
    xAccl = ((data[1] * 256) + data[0]);
    if(xAccl > 32767)
    {
        xAccl -= 65536;
    }
    
    yAccl = ((data[3] * 256) + data[2]);
    if(yAccl > 32767)
    {
        yAccl -= 65536;
    }
    
    zAccl = ((data[5] * 256) + data[4]);
    if(zAccl > 32767)
    {
        zAccl -= 65536;
    }

    // Output data to dashboard
    Serial.print("Acceleration in X-Axis :");
    Serial.println(String(xAccl));
    Serial.print("Acceleration in Y-Axis :");
    Serial.println(String(yAccl));
    Serial.print("Acceleration in Z-Axis :");
    Serial.println(String(zAccl));
    delay(500);
}

