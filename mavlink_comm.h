#ifndef MAVLINK_COMM_H
#define MAVLINK_COMM_H

#include <HardwareSerial.h>
#include <MAVLink_ardupilotmega.h>
#include "config.h"

mavlink_system_t mavlink_system;

#define APM_MODE_STABILIZE  0
#define APM_MODE_ALT_HOLD   2
#define APM_MODE_LOITER     5
#define APM_MODE_GUIDED     4
#define APM_MODE_LAND       9

class MavlinkComm {
private:
    HardwareSerial* mavSerial;
    unsigned long   lastHeartbeatSent;
    unsigned long   lastHeartbeatRecv;
    uint8_t         apmMode;
    bool            apmConnected;
    bool            apmArmed;
    uint8_t         targetSysId;
    uint8_t         targetCompId;

    void sendPacket(mavlink_message_t* msg) {
        uint8_t buf[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_buffer(buf, msg);
        mavSerial->write(buf, len);
    }

public:
    void init() {
        mavSerial = &Serial2; 
        mavSerial->begin(MAV_BAUD, SERIAL_8N1, MAV_RX_PIN, MAV_TX_PIN);
        
        mavlink_system.sysid  = GCS_SYS_ID;
        mavlink_system.compid = GCS_COMP_ID;

        lastHeartbeatSent = 0;
        lastHeartbeatRecv = 0;
        apmMode           = 255;
        apmConnected      = false;
        apmArmed          = false;
        targetSysId       = APM_SYS_ID;
        targetCompId      = APM_COMP_ID;
        Serial.println("[MAV] MAVLink Initialized");
    }

    void arm(bool force = false) {
        mavlink_message_t msg;
        mavlink_msg_command_long_pack(GCS_SYS_ID, GCS_COMP_ID, &msg, targetSysId, 1, MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, force ? 21196 : 0, 0,0,0,0,0);
        sendPacket(&msg);
        Serial.println("[MAV] ARM command sent");
    }

    void disarm() {
        mavlink_message_t msg;
        mavlink_msg_command_long_pack(GCS_SYS_ID, GCS_COMP_ID, &msg, targetSysId, targetCompId, MAV_CMD_COMPONENT_ARM_DISARM, 0, 0, 0,0,0,0,0,0);
        sendPacket(&msg);
        Serial.println("[MAV] DISARM command sent");
    }

    void setMode(uint8_t mode) {
        mavlink_message_t msg;
        mavlink_msg_set_mode_pack(GCS_SYS_ID, GCS_COMP_ID, &msg, targetSysId, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, (uint32_t)mode);
        sendPacket(&msg);
    }

    // FIXED: Added remaining channel arguments (up to 18) to match your MAVLink version
    void sendRC(uint16_t roll, uint16_t pitch, uint16_t throttle, uint16_t yaw) {
        mavlink_message_t msg;
        mavlink_msg_rc_channels_override_pack(
            GCS_SYS_ID, GCS_COMP_ID, &msg,
            targetSysId, targetCompId,
            roll, pitch, throttle, yaw,
            0, 0, 0, 0, // Ch 5-8
            0, 0, 0, 0, // Ch 9-12
            0, 0, 0, 0, // Ch 13-16
            0, 0        // Ch 17-18
        );
        sendPacket(&msg);
    }

    void stopMovingRC() {
        sendRC(1500, 1500, 0, 1500); 
    }

    void heartbeat() {
        if (millis() - lastHeartbeatSent > MAV_HEARTBEAT_MS) {
            mavlink_message_t msg;
            mavlink_msg_heartbeat_pack(GCS_SYS_ID, GCS_COMP_ID, &msg, MAV_TYPE_GCS, MAV_AUTOPILOT_INVALID, 0, 0, MAV_STATE_ACTIVE);
            sendPacket(&msg);
            lastHeartbeatSent = millis();
        }
        parseIncoming();
    }

    void parseIncoming() {
        mavlink_message_t msg;
        mavlink_status_t  status;
        while (mavSerial->available()) {
            uint8_t byte = mavSerial->read();
            if (mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &status)) {
                if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
                    mavlink_heartbeat_t hb;
                    mavlink_msg_heartbeat_decode(&msg, &hb);
                    if (!apmConnected) {
                        targetSysId = msg.sysid;
                        Serial.printf("[MAV] Link established with SysID: %d\n", targetSysId);
                    }
                    apmMode = hb.custom_mode;
                    apmConnected = true;
                    lastHeartbeatRecv = millis();
                    apmArmed = (hb.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) != 0;
                }
            }
        }
    }

    bool    isArmed()     { return apmArmed; }
    uint8_t getMode()      { return apmMode; }
    
    String getModeName() {
        switch (apmMode) {
            case APM_MODE_STABILIZE: return "STABILIZE";
            case APM_MODE_ALT_HOLD:  return "ALT_HOLD";
            case APM_MODE_LOITER:    return "LOITER";
            case APM_MODE_GUIDED:    return "GUIDED";
            case APM_MODE_LAND:      return "LAND";
            case 255:                return "DISCONNECTED";
            default:                 return "MODE_" + String(apmMode);
        }
    }
};

#endif
