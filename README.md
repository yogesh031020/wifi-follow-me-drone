\# 📶 WiFi Follow-Me \& Signal Tracking UAV

\*\*RSSI-Based Autonomous Tracking with Safety-Critical LiDAR Failsafes\*\*



\### 🛠️ Implementation Diagram

```mermaid

graph TD

&#x20;   Target\[WiFi Target] ---|RSSI| ESP32\[ESP32 Brain]

&#x20;   LiDAR\[TF-Mini LiDAR] -->|Distance| ESP32

&#x20;   ESP32 -->|MAVLink Hijack| APM\[APM 2.8 FC]



