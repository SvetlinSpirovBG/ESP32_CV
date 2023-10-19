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
typedef struct wifi_connection_t {
    char ssid[256];
    char password[256];
};




//@SV_TODO: Refactor this logic for potential shared memory between the Host & Client
// typedef enum MessageId {
//     TextMessage,
//     ImageBuffer,
//     AnalogData,
//     Invalid = 0xFF
// };

//@SV_TODO: Refactor this logic for potential shared memory between the Host & Client
// Message type for communication with the ESP host
// typedef struct socketMessage {
//     MessageId msgId;        //Type of the incoming/outgoing message.
//     int       payloadSize;  //Size of the payload in the message
//     void*     pPayload;     //Pointer to the payload
// };

// void connectToWifi(); 		 //Connect to a known Wi-Fi network

class ESP32NetworkController
{
private:
    int       m_socketFd;
public:
    /** @brief   Send a message to the host
     *
     *  @param   payloadSize - Size of the payload in bytes
     *  @param   pPayload    - Pointer to the payload to be send
     *
     *  @return  Success or appropriate failure return code.
    */
    esp_err_t   sendMessage(uint8_t* pPayload, size_t payloadSize);
    static void connectToWifi();
    wl_status_t getWirelessStatus();
    esp_err_t   connectToHostSocket();

    ESP32NetworkController(/* args */);
    ~ESP32NetworkController();


    // esp_err_t   configureCamera(framesize_t fz, pixformat_t pf);
    // esp_err_t   initializeCamera();
    // esp_err_t   captureFrame();
    // pixformat_t getPixelFormat();
};

#endif /* _ESP32_WIFI_CONTROLLER_H_ */
