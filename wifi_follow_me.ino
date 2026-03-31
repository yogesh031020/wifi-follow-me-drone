#include "config.h"
#include "mavlink_comm.h"
#include "lidar.h"
#include "wifi_tracking.h"
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <ESPmDNS.h>

MavlinkComm mav;
Lidar       lidar;
WiFiTracker tracker;
WebServer   server(80);

unsigned long lastLogTime = 0;
String webLog = "";

void handleRoot() {
    String html = "<html><head><meta http-equiv='refresh' content='1'></head><body style='font-family:sans-serif;'>";
    html += "<h1>ESP32 Drone Dashboard</h1>";
    html += "<p><b>Mode:</b> " + mav.getModeName() + "</p>";
    html += "<p><b>Armed:</b> " + String(mav.isArmed() ? "YES" : "NO") + "</p>";
    html += "<p><b>RSSI:</b> " + String(tracker.getRSSI(), 1) + " dBm</p>";
    html += "<p><b>Distance:</b> " + String(lidar.getDistance()) + " cm</p>";
    
    html += "<h3>Live Logs:</h3>";
    html += "<textarea style='width:100%; height:200px; font-family:monospace;' readonly>" + webLog + "</textarea>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("--- WiFi Follow-Me Sentry (Wireless) ---");
    
    mav.init();
    lidar.init();
    tracker.init(TRACK_SSID); 

    server.on("/", handleRoot);
    server.begin();

    ArduinoOTA.setHostname("follow-me-drone");
    ArduinoOTA.begin();
}

void loop() {
    ArduinoOTA.handle();
    server.handleClient();
    mav.heartbeat();
    lidar.update();
    tracker.scan();

    if (millis() - lastLogTime > 500) {
        String logLine = "MOD: " + mav.getModeName() + 
                        " | ARM: " + String(mav.isArmed() ? "YES" : "NO") + 
                        " | DST: " + String(lidar.getDistance()) + "cm" +
                        " | RSSI: " + String(tracker.getRSSI(), 1) + "\n";
        
        Serial.print(logLine);
        webLog = logLine + webLog; 
        if (webLog.length() > 2000) webLog = webLog.substring(0, 2000);
        lastLogTime = millis();
    }

    if (mav.getMode() == 2 && mav.isArmed()) { 
        uint16_t rcPitch = 1500;
        uint16_t rcYaw   = 1500;

        if (lidar.getDistance() > 0 && lidar.getDistance() < OBSTACLE_CLOSE_CM) {
            rcPitch = 1600; 
        } 
        else if (tracker.isFound()) {
            float rssi = tracker.getRSSI();
            if (rssi < (FOLLOW_TARGET_RSSI - FOLLOW_DEADZONE)) rcPitch = 1450;
            else if (rssi > (FOLLOW_TARGET_RSSI + FOLLOW_DEADZONE)) rcPitch = 1550;
        }
        mav.sendRC(1500, rcPitch, 0, rcYaw);
    } else {
        mav.stopMovingRC();
    }
}
