# WiFi Follow-Me Sentry Drone

This project transforms a standard RC drone into an autonomous "Follow-Me" sentry using an **ESP32**, an **APM 2.8** flight controller, and a **TF Mini LiDAR**. The ESP32 acts as an intelligent companion computer, communicating with the APM via MAVLink to autonomously control pitch and yaw based on WiFi signal strength (RSSI) from a target hotspot (like your smartphone).

## 🌟 Key Features
- **WiFi RSSI Tracking:** The drone connects to a target WiFi hotspot and monitors the RSSI (signal strength) to determine distance. It autonomously pitches forward to follow or backward to maintain a safe distance.
- **LiDAR Obstacle Avoidance:** Integrates a TF Mini LiDAR sensor. If an obstacle is detected within the danger zone, the drone will automatically pitch backward to avoid a collision.
- **MAVLink Integration:** The ESP32 fully interfaces with the APM 2.8 via MAVLink, sending heartbeat packets, requesting ARM/DISARM, monitoring flight modes, and sending RC override commands to fly the drone in `ALT_HOLD` mode.
- **Web Dashboard:** Hosts an asynchronous web server on the ESP32. When connected, you can view the drone's live telemetry (Arm state, Flight Mode, RSSI, LiDAR distance, and Logs) from any web browser.
- **Over-The-Air (OTA) Updates:** Flashing new firmware to the ESP32 can be done wirelessly via WiFi, meaning you don't need to plug in a USB cable once the drone is assembled.

## 🛠️ Hardware Requirements
1. **ESP32 Development Board** (NodeMCU or similar)
2. **APM 2.8 Flight Controller** (or Pixhawk, configured for MAVLink telemetry)
3. **TF Mini LiDAR** (UART version)
4. **Drone Frame & Components** (Motors, ESCs, Props, Battery, RC Receiver)
5. **Smartphone** (to act as the WiFi Hotspot target)

## 🔌 Wiring Guide

**ESP32 to TF Mini LiDAR (Serial1):**
- LiDAR TX -> ESP32 GPIO 4 (LIDAR_RX)
- LiDAR RX -> ESP32 GPIO 2 (LIDAR_TX)
- 5V -> 5V (from BEC)
- GND -> GND

**ESP32 to APM 2.8 Telemetry Port (Serial2):**
- APM TX -> ESP32 GPIO 16 (MAV_RX)
- APM RX -> ESP32 GPIO 17 (MAV_TX)
- GND -> GND

*Note: Ensure common ground between all components. The ESP32 and LiDAR should be powered by a stable 5V BEC.*

## 💻 Software Setup
This project is built using the Arduino IDE. 

### Dependencies:
You will need to install the following libraries via the Library Manager or ZIP:
- `MAVLink_ardupilotmega` (Standard MAVLink C headers)
- `WiFi` (Built-in ESP32)
- `WebServer` (Built-in ESP32)
- `ESPmDNS` (Built-in ESP32)
- `ArduinoOTA` (Built-in ESP32)

### Configuration:
Before uploading, open `config.h` and configure:
1. `TRACK_SSID`: Your phone's Mobile Hotspot name.
2. `TRACK_PASS`: Your phone's Mobile Hotspot password.

## 🚀 How to Use
1. **Turn on your Phone's Hotspot** matching the credentials in `config.h`.
2. **Power up the drone**. The ESP32 will boot and connect to your phone.
3. **Open the Dashboard**: Find the ESP32's IP address on your phone's hotspot manager, and open it in a browser (e.g., `http://192.168.43.x`). You will see live telemetry.
4. **Takeoff**: Manually arm and take off using your RC transmitter. Switch the APM to `ALT_HOLD` mode.
5. **Autonomy**: Once in `ALT_HOLD` (Mode 2) and armed, the ESP32 takes over the Pitch and Yaw RC channels to follow your phone and avoid obstacles using the LiDAR!

## 📁 Project Structure
- `wifi_follow_me.ino`: Main runtime loop, dashboard, and OTA handling.
- `config.h`: Pin definitions, thresholds, and MAVLink IDs.
- `mavlink_comm.h`: Handles APM parsing, RC overrides, and flight mode state.
- `lidar.h`: TF Mini UART data parser.
- `wifi_tracking.h`: WiFi connection state and RSSI filtering.

---
*Disclaimer: Autonomous drones are dangerous. Always test in an open, safe environment without people nearby. Keep your RC transmitter ready to switch back to STABILIZE mode or disarm in case of an emergency.*
