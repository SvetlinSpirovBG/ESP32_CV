/**
 * @file esp32_main_controller.hpp
 *
 * @brief ESP32 Main microcontroller class. This class will handle the operation of subsystems
 * like:
 * 1. Camera
 * 2. Wi-Fi
*/

#ifndef _ESP32_MICROCONTROLLER_H_
#define _ESP32_MICROCONTROLLER_H_

#include "esp_dev_lib.hpp"
#include "cam_controller.h"
#include "wifi_controller.h"

#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

class Esp32Microntroller
{
private:
public:
    mutex m_camMtx;
    mutex m_nwMtx;
    ESP32CameraController  m_camController;     //Camera controller. @SV_TODO Expose through an API
    ESP32NetworkController m_networkController; //Network controller. @SV_TODO Expose through an API

    /** @brief      Initialize microcontroller camera's with desired frame size and pixel format
     *
     *  @param fz    Frame size for the captured frames
     *  @param pf    Image pixel format (or color space.) YUV_420, RAW888, GRAYSCALE etc.
     *
     *  @return      Success or failure code
    */
    esp_err_t initializeCamera(framesize_t fz, pixformat_t pf);

    /** @brief      Connect to a known WiFi network
     *
     *  @return      None. This function should loop forever, keeping track of the network status.
    */
    void connectToWifi();

    /** @brief      Start a camera HTTP server running on this microcontroller
     *
     *  @return     Success or failure code
    */
    esp_err_t startCamServer();

    /** @brief      Connect to an HHTP server using IP address and port
     *
     *  @return     Success or failure code
    */
    esp_err_t connectToServer();

    Esp32Microntroller(/* args */);
    ~Esp32Microntroller();
};

extern Esp32Microntroller g_microcontroller;

#endif /* _ESP32_MICROCONTROLLER_H_ */
