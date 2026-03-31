#ifndef LIDAR_H
#define LIDAR_H

#include <Arduino.h>
#include "config.h"

class Lidar {
private:
    HardwareSerial* lidarSerial;
    uint16_t distance;
    uint16_t strength;

public:
    void init() {
        lidarSerial = &Serial1;
        lidarSerial->begin(LIDAR_BAUD, SERIAL_8N1, LIDAR_RX_PIN, LIDAR_TX_PIN); 
        distance = 0;
        strength = 0;
        Serial.println("[Lidar] Initialized on GPIO 4/2");
    }

    void update() {
        while (lidarSerial->available() >= 9) {
            if (lidarSerial->read() == 0x59 && lidarSerial->read() == 0x59) {
                uint8_t d_l = lidarSerial->read();
                uint8_t d_h = lidarSerial->read();
                uint8_t s_l = lidarSerial->read();
                uint8_t s_h = lidarSerial->read();
                distance = d_l + (d_h << 8);
                strength = s_l + (s_h << 8);
                for(int i=0; i<3; i++) lidarSerial->read(); // Skip rest of packet
            }
        }
    }

    uint16_t getDistance() { return distance; }
};

#endif
