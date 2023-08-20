/**
 * @file cam_controller.cpp
 *
 * @brief ESP32 Camera controller
*/

#include "cam_controller.h"

#include <Arduino.h>

#include "esp_dev_lib.hpp"


/**
 * @brief Setup the ESP32 camera configuration
 *
 * @param   fz  Output frame size
 * @param   pf  Output pixel format
 *
 * //@SV_TODO: Add this all in a class and no need for ptr
*/
esp_err_t ESP32CameraController::configureCamera(framesize_t fz, pixformat_t pf)
{
    esp_err_t rc = ESP_OK;

    if (rc == ESP_OK)
    {
        m_config.ledc_channel = LEDC_CHANNEL_0;
        m_config.ledc_timer   = LEDC_TIMER_0;
        m_config.pin_d0       = Y2_GPIO_NUM;
        m_config.pin_d1       = Y3_GPIO_NUM;
        m_config.pin_d2       = Y4_GPIO_NUM;
        m_config.pin_d3       = Y5_GPIO_NUM;
        m_config.pin_d4       = Y6_GPIO_NUM;
        m_config.pin_d5       = Y7_GPIO_NUM;
        m_config.pin_d6       = Y8_GPIO_NUM;
        m_config.pin_d7       = Y9_GPIO_NUM;
        m_config.pin_xclk     = XCLK_GPIO_NUM;
        m_config.pin_pclk     = PCLK_GPIO_NUM;
        m_config.pin_vsync    = VSYNC_GPIO_NUM;
        m_config.pin_href     = HREF_GPIO_NUM;
        m_config.pin_sscb_sda = SIOD_GPIO_NUM;
        m_config.pin_sscb_scl = SIOC_GPIO_NUM;
        m_config.pin_pwdn     = PWDN_GPIO_NUM;
        m_config.pin_reset    = RESET_GPIO_NUM;
        m_config.xclk_freq_hz = 20000000;
        m_config.grab_mode    = CAMERA_GRAB_WHEN_EMPTY;
        m_config.fb_location  = CAMERA_FB_IN_DRAM;
        m_config.jpeg_quality = 12;
        m_config.fb_count     = 1;
        m_config.frame_size   = fz;
        m_config.pixel_format = pf; // JPEG for web streaming
    }

    return rc;
}

/**
 * @brief Initialize the ESP32 camera driver. Wrapper function to esp_camera_init(camera_config_t).
*/
esp_err_t ESP32CameraController::initializeCamera()
{
    esp_err_t rc = ESP_OK;
	TimestampMeter kpiMeter;

    rc = esp_camera_init(&m_config);

    if (rc != ESP_OK)
    {
        esp_log("Failed to initialize uC camera");
        return rc;
    }

	m_pSensor = esp_camera_sensor_get();

	// drop down frame size for higher initial frame rate
	if (m_config.pixel_format == PIXFORMAT_JPEG)
	{
		m_pSensor->set_framesize(m_pSensor, FRAMESIZE_QVGA);
	}

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
	m_pSensor->set_vflip(m_pSensor, 1);
	m_pSensor->set_hmirror(m_pSensor, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
	m_pSensor->set_vflip(m_pSensor, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_BUILTIN_GPIO_NUM)
	setupLedFlash(LED_BUILTIN_GPIO_NUM);
#endif

    ESP_LOG_CONFIG("Camera initialized for %dms", kpiMeter.getResultInMs());

    return rc;
}

/**
 * @brief Get the pixel format
 *
 * @return The output pixel format
*/
pixformat_t ESP32CameraController::getPixelFormat()
{
    esp_log("Pixel format is %d", m_config.pixel_format);
    return m_config.pixel_format;
};

/**
 * @brief Capture a frame
 *
 * @return ESP_OK if successfull or appropriate failure code.
*/
esp_err_t ESP32CameraController::captureFrame() {
    //capture a frame
    camera_fb_t* fb = esp_camera_fb_get();
    if (fb == NULL) {
        esp_log("Frame buffer could not be acquired");
        return ESP_FAIL;
    }

     esp_log("Buffer format:%d, %dx%d", fb->format, fb->width, fb->height);
    //replace this with your own function
    // display_image(fb->width, fb->height, fb->pixformat, fb->buf, fb->len);

    esp_log("Dumping grayscale buffer...");
    for (int row = 0; row < fb->height; row++)
    {
        for (int col = 0; col < fb->width; col++)
        {
            printf("pixel %5d [%3d]\n", (row * fb->height) + col,   *(fb->buf + (row * fb->height) + col));
        }
    }

    //return the frame buffer back to be reused
    esp_camera_fb_return(fb);

    return ESP_OK;
}

ESP32CameraController::ESP32CameraController(/* args */)
{
}

ESP32CameraController::~ESP32CameraController()
{
}
