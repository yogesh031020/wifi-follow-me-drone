#ifndef WIFI_TRACKING_H
#define WIFI_TRACKING_H

#include <WiFi.h>
#include "config.h"

class WiFiTracker {
private:
    float  filteredRSSI;
    bool   found;

public:
    void init(String ssid) {
        filteredRSSI = -100.0;
        found = false;
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        Serial.println("[WiFi] Tracker Initialized");
    }

    void scan() {
        wl_status_t status = WiFi.status();
        if (status == WL_CONNECTED) {
            static bool printedIP = false;
            if (!printedIP) {
                Serial.print("[WiFi] Connected! Dashboard URL: http://");
                Serial.println(WiFi.localIP());
                printedIP = true;
            }
            float currentRSSI = WiFi.RSSI();
            filteredRSSI = (filteredRSSI * 0.8) + (currentRSSI * 0.2);
            found = true;
        } else {
            static unsigned long lastBegin = 0;
            if (millis() - lastBegin > 30000) {
                lastBegin = millis();
                WiFi.begin(TRACK_SSID, TRACK_PASS);
                Serial.printf("[WiFi] Connecting to SSID: %s...\n", TRACK_SSID);
            }
            found = false;
        }
    }

    float getRSSI() { return filteredRSSI; }
    bool isFound()  { return found; }
};

#endif
