#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "SparkFunCCS811.h"

#define CCS811_ADDR 0x5A //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

CCS811 mySensor(CCS811_ADDR);

void setup()
{
    Serial.begin(9600);
    Serial.println("CCS811 Basic Example");

    Wire.begin(); //Compilation will fail here if your hardware doesn't support additional Wire ports

    //This begins the CCS811 sensor and prints error status of .beginWithStatus()
    CCS811Core::CCS811_Status_e returnCode = mySensor.beginWithStatus(Wire); //Pass Wire1 into the library
    Serial.print("CCS811 begin exited with: ");
    Serial.println(mySensor.statusString(returnCode));
}

void loop()
{
    //Check to see if data is ready with .dataAvailable()
    if (mySensor.dataAvailable())
    {
        //If so, have the sensor read and calculate the results.
        //Get them later
        mySensor.readAlgorithmResults();

        Serial.print("CO2[");
        //Returns calculated CO2 reading
        Serial.print(mySensor.getCO2());
        Serial.print("] tVOC[");
        //Returns calculated TVOC reading
        Serial.print(mySensor.getTVOC());
        Serial.print("] millis[");
        //Simply the time since program start
        Serial.print(millis());
        Serial.print("]");
        Serial.println();
    }

    delay(10); //Don't spam the I2C bus
}