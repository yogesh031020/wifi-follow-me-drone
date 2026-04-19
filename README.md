\# ðŸ“¶ WiFi Follow-Me \& Signal Tracking UAV

\*\*RSSI-Based Autonomous Tracking with Safety-Critical LiDAR Failsafes\*\*



\### ðŸ› ï¸ Implementation Diagram

```mermaid

graph TD

    Target\[WiFi Target] ---|RSSI| ESP32\[ESP32 Brain]

    LiDAR\[TF-Mini LiDAR] -->|Distance| ESP32

    ESP32 -->|MAVLink Hijack| APM\[APM 2.8 FC]



