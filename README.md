# 📡 WiFi Follow-Me Drone

![Status](https://img.shields.io/badge/Status-Proof%20of%20Concept-lightgrey)
![Connectivity](https://img.shields.io/badge/Tech-WiFi%20RSSI%20Tracking-blue)
![Microcontroller](https://img.shields.io/badge/MCU-ESP32-white)
![Logic](https://img.shields.io/badge/Logic-Fuzzy%20Control-green)

# WiFi Follow-Me Drone

GPS-free follow-me system running on an ESP32 companion computer. The drone tracks a WiFi access point by RSSI signal strength and issues MAVLink RC overrides to an APM 2.8 flight controller. TF-Mini LiDAR handles obstacle avoidance. Includes a live web dashboard and OTA firmware updates over the same WiFi link.

## How it works

The ESP32 continuously scans for the target SSID and reads the RSSI. Based on signal strength relative to a setpoint, it adjusts the pitch RC channel — fly closer when signal is weak, hold when in the deadzone, pull back when signal is strong. The LiDAR adds a hard obstacle brake: if anything is within `OBSTACLE_CLOSE_CM`, pitch is overridden to move away.

```
Target WiFi AP → RSSI scan (tracker)
TF-Mini LiDAR  → distance read (lidar)
                      ↓
            ESP32 decision logic
                      ↓
          MAVLink RC override → APM 2.8
```

The system only sends RC overrides when the APM is in mode 2 (Stabilize) and armed — safe by default.

## Live web dashboard

The ESP32 runs a small Flask-style web server on port 80. Visit `http://follow-me-drone.local` in a browser to see live mode, arm state, RSSI, LiDAR distance, and a scrolling log — auto-refreshing every second. No app needed.

## Hardware

| Component | Role |
|---|---|
| ESP32 Dev Module | Companion computer — WiFi + logic |
| APM 2.8 | Flight controller |
| TF-Mini LiDAR | Forward obstacle detection |
| Target WiFi AP | The device being followed |

## Hardware Showcase
![WiFi Follow-Me Drone](Wifi-follow-me-drone.jpg)


## Configuration

Edit `config.h` before flashing:

```cpp
#define TRACK_SSID "your-phone-hotspot"
#define FOLLOW_TARGET_RSSI   -55.0   // dBm setpoint
#define FOLLOW_DEADZONE       5.0    // ±dBm tolerance
#define OBSTACLE_CLOSE_CM    80      // brake distance
```

## Flash

Arduino IDE 2.x, board: ESP32 Dev Module. Libraries needed: MAVLink, ArduinoOTA, WebServer, ESPmDNS.

## What I learned

Raw RSSI is noisy — even standing still the value jumps ±8 dBm per scan. Added a Kalman filter in `wifi_tracking.h` which smoothed it enough to stop the drone oscillating. Also reduced the MAVLink polling rate when the web server was under load — high CPU caused RC override packets to drop.

## Status

Working. Testing with 5GHz band next — expect better directionality for the RSSI signal.
