#include <Arduino.h>
#include "MeasurementAggregator.hpp"
#include <HTTPClient.h>
#include "WiFiHelper.hpp"
#include "config.hpp"
#include "EditableTFTString.hpp"
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <User_Setup.h>
#include <ArduinoJson.h>

String server = "http://192.168.178.53:8085/";
String uuidWiFi = WiFi.macAddress();
String envPostURI = "env/iot";

MeasurementAggregator aggregator;

#ifndef noDisplay
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

EditableTFTString uuid(5, 5 + (tft.fontHeight() + 2) * 0, "Device-ID: ", uuidWiFi, &tft);
EditableTFTString temp(5, 5 + (tft.fontHeight() + 2) * 1, "Temperature: ", "0C", &tft);
EditableTFTString hum(5, 5 + (tft.fontHeight() + 2) * 2, "Humidity: ", "0%", &tft);
EditableTFTString pres(5, 5 + (tft.fontHeight() + 2) * 3, "Pressure: ", "0hPa", &tft);
EditableTFTString co2(5, 5 + (tft.fontHeight() + 2) * 4, "CO2: ", "0", &tft);
EditableTFTString tvoc(5, 5 + (tft.fontHeight() + 2) * 5, "TVOC: ", "0", &tft);
EditableTFTString height(5, 5 + (tft.fontHeight() + 2) * 6, "Height Approx.: ", "0m", &tft);
EditableTFTString error(5, 5 + (tft.fontHeight() + 2) * 10, "", "", &tft);

#ifdef ILI9341_DRIVER
struct DeviceSet
{
  EditableTFTString list[7];
  String uuid;
};
int numDevices = 0;
#define maxDeviceListLength 3
DeviceSet deviceList[maxDeviceListLength];
DynamicJsonDocument doc(8096);
#endif

int tftHeight = 0;
int tftWidth = 0;
#endif

String postToServer()
{
  if (connectedWiFi())
  {
    HTTPClient http;

    String url = server + envPostURI + "/" + uuidWiFi + "/?humidity=" + String(aggregator.getHumidity()) + "&temperature=" + String(aggregator.getTemperature()) + "&pressure=" + String(aggregator.gethPaPressure()) + "&co2=" + String(aggregator.getCO2Value()) + "&tvoc=" + String(aggregator.getTVOCValue()) + "&heightapprox=" + String(aggregator.getHeightApprox());

    http.begin(url.c_str());
    int response = http.POST("");
    if (response > 0)
    {
      //Serial.print("HTTP Response code: ");
      //Serial.println(response);
      String responseString = http.getString();
      http.end();
      return responseString;
      //aSerial.println(payload);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(response);
#ifndef noDisplay
      if (response == -1)
      {
        error.updateString("Cannot establish server connection");
      }
      else
        error.updateString("Server error code:" + String(response));
#endif
    }
    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
  return "";
}

void redrawAll()
{
  tft.fillScreen(TFT_BLACK);

  tftHeight = tft.height();
  tftWidth = tft.width();

  uuid.drawAllNew();
  temp.drawAllNew();
  hum.drawAllNew();
  pres.drawAllNew();
  co2.drawAllNew();
  tvoc.drawAllNew();
  height.drawAllNew();
}

void setup()
{
  Serial.begin(9600);
  delay(100);
  aggregator.init();

#ifndef noDisplay
  tft.init();
  tft.setRotation(3);
#ifdef ILI9341_DRIVER
  tft.setRotation(2);
#endif

  redrawAll();
  error.updateString("WiFi not connected yet");
#endif

  connectWiFi();
#ifndef noDisplay
  error.clearAll();
#endif
}

void loop()
{
  aggregator.loop();
  //aggregator.printToSerial();
  String json = postToServer();

#ifndef noDisplay
  tft.setTextColor(TFT_WHITE);

  //update all values in tft

  temp.updateString((aggregator.getTemperature() > -500.0F) ? String(aggregator.getTemperature()) + "C" : "Na");
  delay(50);

  hum.updateString((aggregator.getHumidity() >= 0) ? String(aggregator.getHumidity()) + "%" : "Na");
  delay(50);

  pres.updateString((aggregator.gethPaPressure() >= 0) ? String(aggregator.gethPaPressure()) + "hPa" : "Na");
  delay(50);

  co2.updateString((aggregator.getCO2Value() >= 0) ? String(aggregator.getCO2Value()) : "Na");
  delay(50);

  tvoc.updateString((aggregator.getTVOCValue() >= 0) ? String(aggregator.getTVOCValue()) : "Na");
  delay(50);

  height.updateString((aggregator.getHeightApprox() > -infinityf()) ? String(aggregator.getHeightApprox()) + "m" : "Na");
  delay(50);

#ifdef ILI9341_DRIVER
  DeserializationError error = deserializeJson(doc, json);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  long listSize = doc["listSize"];
  for (int i = 0; i < listSize && i < maxDeviceListLength; i++)
  {
    String uuid = doc["uuids"][i];
    if (uuid == uuidWiFi)
      continue;
    DeviceSet *device = nullptr;
    int index = 0;
    //search right Device
    for (int j = 0; j < numDevices; j++)
    {
      if (deviceList[j].uuid == uuid)
      {
        device = &deviceList[j];
        index = j;
      }
    }

    //if device not exist
    if (device == nullptr)
    {
      device = new DeviceSet();
      device->uuid = uuid;
      deviceList[numDevices] = *device;
      delete device;
      device = &deviceList[numDevices];
      index = numDevices + 1;
      numDevices++;

      device->list[0].setTFT(&tft);
      device->list[0].setXY(5, 5 + (tft.fontHeight() + 2) * 8 * index + (tft.fontHeight() + 2) * 0);
      device->list[0].setPermanentString("Device-ID: ");
      device->list[0].updateString(uuid);

      device->list[1].setTFT(&tft);
      device->list[1].setXY(5, 5 + (tft.fontHeight() + 2) * 8 * index + (tft.fontHeight() + 2) * 1);
      device->list[1].setPermanentString("Temperature: ");

      device->list[2].setTFT(&tft);
      device->list[2].setXY(5, 5 + (tft.fontHeight() + 2) * 8 * index + (tft.fontHeight() + 2) * 2);
      device->list[2].setPermanentString("Humidity: ");

      device->list[3].setTFT(&tft);
      device->list[3].setXY(5, 5 + (tft.fontHeight() + 2) * 8 * index + (tft.fontHeight() + 2) * 3);
      device->list[3].setPermanentString("Pressure: ");

      device->list[4].setTFT(&tft);
      device->list[4].setXY(5, 5 + (tft.fontHeight() + 2) * 8 * index + (tft.fontHeight() + 2) * 4);
      device->list[4].setPermanentString("CO2: ");

      device->list[5].setTFT(&tft);
      device->list[5].setXY(5, 5 + (tft.fontHeight() + 2) * 8 * index + (tft.fontHeight() + 2) * 5);
      device->list[5].setPermanentString("TVOC: ");

      device->list[6].setTFT(&tft);
      device->list[6].setXY(5, 5 + (tft.fontHeight() + 2) * 8 * index + (tft.fontHeight() + 2) * 6);
      device->list[6].setPermanentString("Height Approx.: ");
    }

    float temp = doc["valueMap"][device->uuid][0]["temperature"];
    float hum = doc["valueMap"][device->uuid][0]["humidity"];
    float pres = doc["valueMap"][device->uuid][0]["pressure"];
    int32_t co2 = doc["valueMap"][device->uuid][0]["co2"];
    int32_t tvoc = doc["valueMap"][device->uuid][0]["tvoc"];
    float height = doc["valueMap"][device->uuid][0]["heightApproximation"];
    device->list[1].updateString((temp > -500.0F) ? String(temp) + "C" : "Na");
    device->list[2].updateString((hum >= 0.0F) ? String(hum) + "%" : "Na");
    device->list[3].updateString((pres >= 0.0) ? String(pres) + "C" : "Na");
    device->list[4].updateString((co2 >= 0.0) ? String(co2) : "Na");
    device->list[5].updateString((tvoc >= 0.0F) ? String(tvoc) : "Na");
    device->list[6].updateString((height > -infinityf()) ? String(height) + "m" : "Na");
  }
  doc.clear();
#endif
#else
  delay(300);
#endif
}