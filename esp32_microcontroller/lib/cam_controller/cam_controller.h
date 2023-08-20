/**
 * @file cam_controller.h
 *
 * @brief ESP32 Camera controller
*/

#ifndef _ESP32_CAM_CONTROLLER_H_
#define _ESP32_CAM_CONTROLLER_H_

#include "esp_dev_lib.hpp"
#include "esp_camera.h"

// WARNING!!!
// PSRAM IC required for UXGA resolution and high JPEG quality. Ensure ESP32 Wrover Module or other board with PSRAM is
// selected. Partial images will be transmitted if image exceeds buffer size.
//
// You must select partition scheme from the board menu that has at least 3MB APP space. Face Recognition is DISABLED for ESP32
// and ESP32-S2, because it takes up from 15 seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as
// well.

// ===================
// Select camera model
// ===================
// #define CAMERA_MODEL_WROVER_KIT          // Has PSRAM
// #define CAMERA_MODEL_ESP_EYE             // Has PSRAM
// #define CAMERA_MODEL_ESP32S3_EYE         // Has PSRAM
// #define CAMERA_MODEL_M5STACK_PSRAM       // Has PSRAM
// #define CAMERA_MODEL_M5STACK_V2_PSRAM    // M5Camera version B Has PSRAM
// #define CAMERA_MODEL_M5STACK_WIDE        // Has PSRAM
// #define CAMERA_MODEL_M5STACK_ESP32CAM    // No PSRAM
// #define CAMERA_MODEL_M5STACK_UNITCAM     // No PSRAM
#define CAMERA_MODEL_AI_THINKER             // Has PSRAM
// #define CAMERA_MODEL_TTGO_T_JOURNAL      // No PSRAM
//  ** Espressif Internal Boards **
// #define CAMERA_MODEL_ESP32_CAM_BOARD
// #define CAMERA_MODEL_ESP32S2_CAM_BOARD
// #define CAMERA_MODEL_ESP32S3_CAM_LCD
#include "camera_pins.h"

class ESP32CameraController
{
private:
    camera_config_t m_config;
    sensor_t* m_pSensor;
public:
    ESP32CameraController(/* args */);
    ~ESP32CameraController();


    esp_err_t   configureCamera(framesize_t fz, pixformat_t pf);
    esp_err_t   initializeCamera();
    esp_err_t   captureFrame();
    pixformat_t getPixelFormat();
};

#endif /* _ESP32_CAM_CONTROLLER_H_ */
