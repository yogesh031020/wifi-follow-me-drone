# ?? WiFi Follow-Me & Signal Tracking UAV
**RSSI-Based Autonomous Tracking with Safety-Critical LiDAR Failsafes**

### ?? Project Overview
A custom "Follow-Me" drone that uses **RSSI (Received Signal Strength Indication)** from a mobile device or transmitter to maintain a set distance.

### ??? Implementation Diagram
\\\mermaid
graph TD
    Target[WiFi Target] ---|RSSI| ESP32[ESP32 Brain]
    LiDAR[TF-Mini LiDAR] -->|Distance| ESP32
    ESP32 -->|MAVLink Hijack| APM[APM 2.8 FC]
\\\

### ?? Core Features
*   **RSSI Tracking:** Dynamic distance maintenance based on WiFi signal strength.
*   **RC Channel Hijacking:** ESP32-based MAVLink packet manipulation.
*   **Collision Failsafe:** Integrated TF-Mini LiDAR to override tracking.
