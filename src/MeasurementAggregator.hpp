#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "SparkFunCCS811.h"
#include <Adafruit_BME280.h>
#include "config.hpp"

class MeasurementAggregator
{
public:
    MeasurementAggregator();
    /**
     * @brief Initializes all measuring components
     * 
     * @return Returns if initialisation of all components was successfull
     */
    bool init();
    /**
     * @brief run your loop function before read values to get newest data
     * 
     */
    void loop();

    /**
    * @brief get CO2 value
    * 
    * @return uint16_t 
    */
    uint16_t getCO2Value();
    /**
     * @brief get TVOC value
     * 
     * @return uint16_t 
     */
    uint16_t getTVOCValue();

    /**
    * @brief Get the Temperature in C
    * 
    * @return float 
    */
    float getTemperature();
    /**
     * @brief Get the pressure hPa
     * 
     * @return float 
     */
    float gethPaPressure();
    /**
     * @brief Get the Humidity in %
     * 
     * @return float 
     */
    float getHumidity();
    /**
     * @brief Get a Height Approximation
     * 
     * @return float 
     */
    float getHeightApprox();

private:
    bool initCCS811();
    bool initMBE280();

    CCS811 *airQualitySensor;
    bool airSensorActive = false;
    Adafruit_BME280 *bme;
    bool bmeActive = false;
};