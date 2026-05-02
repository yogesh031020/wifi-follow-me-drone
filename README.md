# 📡 WiFi Follow-Me Drone

![Status](https://img.shields.io/badge/Status-Proof%20of%20Concept-lightgrey)
![Connectivity](https://img.shields.io/badge/Tech-WiFi%20RSSI%20Tracking-blue)
![Microcontroller](https://img.shields.io/badge/MCU-ESP32-white)
![Logic](https://img.shields.io/badge/Logic-Fuzzy%20Control-green)

**Experimental follow-me system using WiFi signal strength (RSSI) triangulation for autonomous pilot tracking without GPS.**

 \# ðŸ“¶ WiFi Follow-Me \& Signal Tracking UAV

\*\*RSSI-Based Autonomous Tracking with Safety-Critical LiDAR Failsafes\*\*



\### ðŸ› ï¸ Implementation Diagram

```mermaid

graph TD

    Target\[WiFi Target] ---|RSSI| ESP32\[ESP32 Brain]

    LiDAR\[TF-Mini LiDAR] -->|Distance| ESP32

    ESP32 -->|MAVLink Hijack| APM\[APM 2.8 FC]



