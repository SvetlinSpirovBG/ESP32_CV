/**
 * @file esp32_main_controller.cpp
 *
 * @brief ESP32 Main microcontroller class. This class will handle the operation of subsystems
 * like:
 * 1. Camera
 * 2. Wi-Fi
*/

#include "esp32_main_controller.hpp"


/** @brief      Asynchronous wrapper function to call a function that is going to connect to a WiFi network
 *
 *  @param      pObj - Pointer to an Esp32Microntroller class object
 *
 *  @return     Success or failure code
*/
void* asyncWifiConn(void* pObj)
{
    Esp32Microntroller* pUc = reinterpret_cast<Esp32Microntroller*>(pObj);
    lock_guard<mutex> networkMutex(pUc->m_nwMtx);
    pUc->m_networkController.connectToWifi();

    return nullptr;
}

Esp32Microntroller::Esp32Microntroller()
{
}

Esp32Microntroller::~Esp32Microntroller()
{
}

esp_err_t Esp32Microntroller::initializeCamera(framesize_t fz, pixformat_t pf)
{
    esp_err_t rc = ESP_OK;

    ESP_LOG_CONFIG("Initializing ESP32 camera with OV2640 image sensor");

    std::lock_guard<std::mutex> lock(m_camMtx);

    rc = this->m_camController.configureCamera(fz, pf);

    if (rc != ESP_OK)
    {
        ESP_LOG_ERROR("Camera configuration failure! rc = %d", rc);
        return rc;
    }

    rc = this->m_camController.initializeCamera();

    if (rc != ESP_OK)
    {
        ESP_LOG_ERROR("Failed to initialize the ESP32 camera! rc = %d", rc);
    }

    return rc;
}

void Esp32Microntroller::connectToWifi()
{
	pthread_attr_t wifi_th_attr;
	pthread_t wifi_thread;

    // Detach the connection to the wifi
	pthread_attr_setstacksize(&wifi_th_attr, 5000);
	pthread_attr_setdetachstate(&wifi_th_attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&wifi_thread, &wifi_th_attr, &asyncWifiConn, this);

    //Lock WiFi mutex and unlock it when WiFi is conected. Camera initialize and then will wait for this mutex
    //before starting the camera server
}

esp_err_t Esp32Microntroller::startCamServer()
{
    esp_err_t rc = ESP_OK;

    lock_guard<mutex> cameraLock(m_camMtx);
    lock_guard<mutex> networkLock(m_nwMtx);

	startCameraServer();

    //@SV_TODO: Add a real rc check
    return rc;
}

/**
 * @brief Wrapper function
 * @SV_TODO: Remove the app_httpd file and move it to a child microcontroller class
*/
esp_err_t Esp32Microntroller::connectToServer()
{
    esp_err_t rc = ESP_FAIL;

    lock_guard<mutex> networkLock(m_nwMtx);

    rc = m_networkController.connectToHostSocket();
    return rc;
}

//External declaration of a global microcontroller.
//@todo: make this a singleton to practice some C++
Esp32Microntroller g_microcontroller;
