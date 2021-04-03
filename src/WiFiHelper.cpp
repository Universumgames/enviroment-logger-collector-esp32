#include "WiFiHelper.hpp"

void connectWiFi()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.mode(WIFI_AP_STA);
        delay(500);

#ifndef WIFI_SSID
        WiFi.beginSmartConfig();
#else
        WiFi.begin(WIFI_SSID, WIFI_PWD);
#endif

        int tries = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
            Serial.println(WiFi.smartConfigDone());
            tries++;
            if (tries > 20)
                ESP.restart();
        }
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

bool connectedWiFi()
{
    return WiFi.status() == WL_CONNECTED;
}