/**
 * @file main.cpp
 *
 * @brief Main???
*/

#include <Arduino.h>

#include "esp_dev_lib.hpp"
#include "wifi_controller.h"
#include "esp32_main_controller.hpp"

static pthread_t monitor_thread;

// A normal C function that is executed as a thread when its name is specified in pthread_create()
void *monitorFunc(void *vargp)
{
	esp_log("ESP32-Cam monitor function started");

	while(0)
	{
		esp_log("Monitor loop...");
		delay(1000);
	}

    return NULL;
}

/**
 * @brief Put your setup code here, to run once
*/
void setup()
{
	esp_err_t rc;
	TimestampMeter bootMeter;

	//@SV_TODO: Add TimestampMeter class, just like the good old days!

	Serial.begin(115200);
	Serial.setDebugOutput(true);

	esp_log("-----------------------------");
	esp_log("ESP32-CAM Microcontroller started!");
	esp_log("-----------------------------");

	//Create monitor thread/s
	pthread_create(&monitor_thread, NULL, monitorFunc, NULL);

	g_microcontroller.connectToWifi(); //Async connection to network

	//Configure LED
	pinMode(LED_BUILTIN_GPIO_NUM, OUTPUT);

#if defined(CAMERA_MODEL_ESP_EYE)
	pinMode(13, INPUT_PULLUP);
	pinMode(14, INPUT_PULLUP);
#endif

	rc = g_microcontroller.initializeCamera(framesize_t::FRAMESIZE_VGA, pixformat_t::PIXFORMAT_JPEG);
	if (rc != ESP_OK)
	{
		esp_log("Camera init failed with error 0x%x", rc);
		return;
	}

	rc = g_microcontroller.startCamServer();
	if (rc != ESP_OK)
	{
		esp_log("Failed to start camera server! Error 0x%x", rc);
		return;
	}
	else
	{
		ESP_LOG_CONFIG("Camera Ready! Use 'http://%s to connect. Boot (timestamp):%dms",
			WiFi.localIP().toString().c_str(), bootMeter.getResultInMs());
	}

	blinkLED(250, 3);
}

void loop()
{
	// put your main code here, to run repeatedly:
	// esp_log("loop...");
	return;
}
