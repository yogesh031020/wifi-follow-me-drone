# WiFi Follow-Me Drone: Hardware Wiring & Pinout Map

This document outlines the physical connections between the ESP32 (Companion Computer), the APM 2.8 flight controller, and the LiDAR sensor.

## 🧠 Core Systems

| Component | Pin / Port | Connection | Notes |
| :--- | :--- | :--- | :--- |
| **ESP32 (Core)** | `5V` / `GND` | Power Distribution Board | Must be powered from a clean 5V BEC. Do not power directly from APM telemetry. |
| **APM 2.8 (Telemetry)**| `Serial2` (TX/RX)| ESP32 `GPIO 16 / 17` | MAVLink communication (Baud: 57600). **Requires 5V to 3.3V Logic Level Shifter.** |

## 📡 Sensor Suite

| Sensor | ESP32 Pin | Protocol/Type | Function |
| :--- | :--- | :--- | :--- |
| **TF-Mini LiDAR** | `Serial1` (TX/RX)| UART | Forward-facing obstacle detection and emergency braking. |
| **ESP32 Internal WiFi** | N/A | 2.4GHz / 5GHz antenna | Scans for target SSID RSSI to determine distance. |

## ⚠️ Important Hardware Notes
1. **Logic Level Shifting**: The APM 2.8 telemetry port operates at 5V logic. The ESP32 is NOT 5V tolerant. You MUST use a Bi-Directional Logic Level Converter between the APM TX and ESP32 RX.
2. **Antenna Placement**: Ensure the ESP32 antenna is not obstructed by the drone's carbon fiber frame or battery, as this will artificially reduce the RSSI readings and cause erratic tracking.
3. **LiDAR Alignment**: The TF-Mini must point directly forward to accurately trigger the `OBSTACLE_CLOSE_CM` brake override.
