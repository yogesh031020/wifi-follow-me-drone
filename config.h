#ifndef CONFIG_H
#define CONFIG_H

// === TF Mini LiDAR (Serial1) ===
#define LIDAR_RX_PIN         4   
#define LIDAR_TX_PIN         2   
#define LIDAR_BAUD           115200

// === MAVLink Serial to APM 2.8 (UART2) ===
#define MAV_RX_PIN           16  
#define MAV_TX_PIN           17  
#define MAV_BAUD             57600

// === WiFi & Target Config ===
#define TRACK_SSID           "test"
#define TRACK_PASS           "wifihotspot123"

// === Thresholds ===
#define OBSTACLE_CLOSE_CM    100   
#define FOLLOW_TARGET_RSSI   -60   
#define FOLLOW_DEADZONE      5     
#define FOLLOW_YAW_STEP      15.0  
#define FOLLOW_YAW_THRESHOLD 3.0   
#define MAV_HEARTBEAT_MS     1000  

// === MAVLink System IDs ===
#define GCS_SYS_ID           255  
#define GCS_COMP_ID          0    
#define APM_SYS_ID           1    
#define APM_COMP_ID          1    

#endif
