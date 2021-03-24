#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <Adafruit_CCS811.h>
#include <Wire.h>

Adafruit_CCS811 ccs;
Adafruit_BME280 bme;

unsigned long delayTime;
#define SEALEVELPRESSURE_HPA (1013.25)

void printValues();

void setup()
{
    Serial.begin(9600);
    Serial.println(F("BME280 test"));

    if (!bme.begin(0x76, &Wire))
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1)
            ;
    }

    Serial.println("-- Default Test --");
    Serial.println("normal mode, 16x oversampling for all, filter off,");
    Serial.println("0.5ms standby period");
    delayTime = 5000;
    Serial.println();
}

void loop()
{
    // Only needed in forced mode! In normal mode, you can remove the next line.
    bme.takeForcedMeasurement(); // has no effect in normal mode

    printValues();
    delay(delayTime);
}

void printValues()
{
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}