/**
 * @file esp32_main_controller.cpp
 *
 * @brief ESP32 Main microcontroller class. This class will handle the operation of subsystems
 * like:
 * 1. Camera
 * 2. Wi-Fi
*/

#include "esp32_main_controller.hpp"

void* asyncWifiConn(void* pObj)
{
    Esp32Microntroller* pUc = reinterpret_cast<Esp32Microntroller*>(pObj);
    lock_guard<mutex>pesho(pUc->m_camMtx);
    pUc->m_networkController.connectToWifi();

    return nullptr;
}

Esp32Microntroller::Esp32Microntroller()
{
}

Esp32Microntroller::~Esp32Microntroller()
{
}

/**
 * @brief Initialize the ESP32 camera driver. Wrapper function to esp_camera_init(camera_config_t).
*/

esp_err_t Esp32Microntroller::initializeCamera(framesize_t fz, pixformat_t pf)
{
    esp_err_t rc = ESP_OK;

    esp_log("Initializing camera...");

    std::lock_guard<std::mutex> gosho(m_camMtx);

    rc = this->m_camController.configureCamera(fz, pf);

    if (rc != ESP_OK)
    {
        esp_log("Camera configuration failure! rc = %d", rc);
        return rc;
    }

    rc = this->m_camController.initializeCamera();

    if (rc != ESP_OK)
    {
        esp_log("Failed to initialize the ESP camera");
    }

    esp_log("Camera init rc %d", rc);

    return rc;
}

void Esp32Microntroller::connectToWifi()
{
	pthread_attr_t wifi_th_attr;
	pthread_t wifi_thread;

    // Detach the connection to the wifi
	pthread_attr_setstacksize(&wifi_th_attr, 5000);
	pthread_attr_setdetachstate(&wifi_th_attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&wifi_thread, &wifi_th_attr, &asyncWifiConn, this);

    //Lock WiFi mutex and unlock it when WiFi is conected. Camera initialize and then will wait for this mutex
    //before starting the camera server


    // lock_guard<mutex>lock(wifi_mutex);

    // m_networkController.connectToWifi();
}

esp_err_t Esp32Microntroller::startCamServer()
{
    esp_err_t rc = ESP_OK;

    lock_guard<mutex>gosho(m_camMtx);
    lock_guard<mutex>pesho(m_nwMtx);

	startCameraServer();

    //@SV_TODO: Add a real rc check
    return rc;
}

//External declaration of a global microcontroller.
//@todo: make this a singleton to practice some C++
Esp32Microntroller g_microcontroller;
