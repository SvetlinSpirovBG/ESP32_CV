/**
 * @file wifi_controller.h
 *
 * @brief ESP32 WiFi controller
*/

#ifndef _ESP32_WIFI_CONTROLLER_H_
#define _ESP32_WIFI_CONTROLLER_H_

#include "esp_wifi.h"
#include <WiFi.h>


#include <Arduino.h>
#include "esp_dev_lib.hpp"
#include "cam_controller.h"
#include "wifi_controller.h"

// Wifi connection type. SSID and password
typedef struct wifi_connection {
    char ssid[256];
    char password[256];
} wifi_connection_t;

// void connectToWifi(); 		 //Connect to a known Wi-Fi network

class ESP32NetworkController
{
private:

public:
    static void connectToWifi();
    wl_status_t getWirelessStatus();

    ESP32NetworkController(/* args */);
    ~ESP32NetworkController();


    // esp_err_t   configureCamera(framesize_t fz, pixformat_t pf);
    // esp_err_t   initializeCamera();
    // esp_err_t   captureFrame();
    // pixformat_t getPixelFormat();
};

#endif /* _ESP32_WIFI_CONTROLLER_H_ */
