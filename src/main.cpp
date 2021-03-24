#include <Arduino.h>
#include "MeasurementAggregator.hpp"

MeasurementAggregator aggregator;

void setup()
{
  Serial.begin(9600);
  aggregator.init();
}

void loop()
{
  aggregator.loop();

  Serial.print(aggregator.getHumidity());
  Serial.print("%, ");
  Serial.print(aggregator.getTemperature());
  Serial.print("°C, ");
  Serial.print(aggregator.gethPaPressure());
  Serial.print(" hPa, ");
  Serial.print(aggregator.getCO2Value());
  Serial.print(" CO2, ");
  Serial.print(aggregator.getTVOCValue());
  Serial.print(" TVOC, ");
  Serial.print(aggregator.getHeightApprox());
  Serial.println(" m");
  delay(300);
}