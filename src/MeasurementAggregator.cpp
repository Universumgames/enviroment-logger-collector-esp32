#include "MeasurementAggregator.hpp"

MeasurementAggregator::MeasurementAggregator() {}

bool MeasurementAggregator::init()
{
    airSensorActive = initCCS811();
    bmeActive = initMBE280();
    return airSensorActive && bmeActive;
}

bool MeasurementAggregator::initCCS811()
{
    Wire.begin();
    airQualitySensor = new CCS811(CCS811_ADDR);
    CCS811Core::CCS811_Status_e returnCode = airQualitySensor->beginWithStatus(Wire); // Pass Wire1 into the library
    Serial.print("CCS811 begin exited with: ");
    Serial.println(airQualitySensor->statusString(returnCode));
    return returnCode == CCS811Core::CCS811_Status_e::CCS811_Stat_SUCCESS;
}

bool MeasurementAggregator::initMBE280()
{
    bme = new Adafruit_BME280();
    if (!bme->begin(0x76))
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        return false;
    }
    return true;
}

void MeasurementAggregator::loop()
{
    if (bmeActive)
        bme->takeForcedMeasurement();
    if (airSensorActive && airQualitySensor->dataAvailable())
        airQualitySensor->readAlgorithmResults();
}

int32_t MeasurementAggregator::getCO2Value()
{
    if (airSensorActive)
    {
        return airQualitySensor->getCO2();
    }
    else
        return -1;
}

int32_t MeasurementAggregator::getTVOCValue()
{
    if (airSensorActive)
        return airQualitySensor->getTVOC();
    else
        return -1;
}

float MeasurementAggregator::getTemperature()
{
    if (bmeActive)
        return bme->readTemperature();
    else
        return -500.0F;
}

float MeasurementAggregator::gethPaPressure()
{
    if (bmeActive)
        return bme->readPressure() / 100.0F;
    else
        return -1.0F;
}

float MeasurementAggregator::getHumidity()
{
    if (bmeActive)
        return bme->readHumidity();
    else
        return -1;
}

float MeasurementAggregator::getHeightApprox()
{
    if (bmeActive)
        return bme->readAltitude(SEALEVELPRESSURE_HPA);
    else
        return -infinityf();
}

void MeasurementAggregator::printToSerial(bool easyRead)
{
    if (easyRead)
    {
        Serial.print(getHumidity());
        Serial.print("%, ");
        Serial.print(getTemperature());
        Serial.print("°C, ");
        Serial.print(gethPaPressure());
        Serial.print(" hPa, ");
        Serial.print(getCO2Value());
        Serial.print(" CO2, ");
        Serial.print(getTVOCValue());
        Serial.print(" TVOC, ");
        Serial.print(getHeightApprox());
        Serial.println(" m");
    }
}