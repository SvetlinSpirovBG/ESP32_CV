/**
 * @file wifi_controller.cpp
 *
 * @brief ESP32 WiFi controller
*/

#include "wifi_controller.h"

#include <Arduino.h>

#include "esp_camera.h"
#include "esp_dev_lib.hpp"

#define WL_MAX_CONN_TRIES 2000

static volatile bool g_wifiConnected = false;

// List of ssid/password pair of known WiFi networks
static const wifi_connection_t known_wifis[] = {
	{
		{.ssid     = "Svetlin's iPhone"},
		{.password = "batearni"},
	},
	// {
	// 	{.ssid 	   = "TP-Link_37DD"},
	// 	{.password = "Toni1998"},
	// },
	// {
	// 	{.ssid 	   = "TP-Link_37DD_5G"},
	// 	{.password = "Toni1998"},
	// },
	// {
	// 	{.ssid     = "MMS_GUESTS"},
	// 	{.password = ""},
	// },
};

/**
 * @brief Interate through all known networks and try to connect until a connection is made
 *
 * @return None
*/
void ESP32NetworkController::connectToWifi()
{
	esp_log("ESP32 WiFi controller");

	TimestampMeter kpiMeter;
	size_t num_networks = sizeof(known_wifis) / sizeof(known_wifis[0]);

	esp_log("Number of known networks = %d", num_networks);
	//Print networks for user information only
	for (size_t i = 0; i < num_networks; i++)
	{
		esp_log("  %s", known_wifis[i].ssid);
	}

	while (WiFi.status() != WL_CONNECTED)
	{
		uint32_t max_conn_attemps;

		if (num_networks > 1)
		{
			int numTries = 5;
			for (size_t i = 1; i <= numTries; i++)
			{
				for (size_t j = 0; j < num_networks; j++)
				{
					max_conn_attemps = WL_MAX_CONN_TRIES * i;

					WiFi.begin(known_wifis[j].ssid, known_wifis[j].password);
					WiFi.setSleep(false);

					// Try to establish connectection for max_conn_attemps, If no connection, try with the next known network
					esp_log("Connecting to network %s...", known_wifis[j].ssid);

					for (int attempt = 0; attempt < max_conn_attemps; attempt++)
					{
						if (WiFi.status() == WL_CONNECTED)
						{
							esp_log("WiFi connected to %s for %ums",
								known_wifis[j].ssid, kpiMeter.getResultInMs());

							return;
						}

						delay(1);
					}
				}
			}
		}
		else
		{
			max_conn_attemps = WL_MAX_CONN_TRIES * 100;

			WiFi.begin(known_wifis[0].ssid, known_wifis[0].password);
			WiFi.setSleep(false);

			esp_log("Connecting to network %s...", known_wifis[0].ssid);

			for (int attempt = 0; attempt < max_conn_attemps; attempt++)
			{
				if (WiFi.status() == WL_CONNECTED)
				{
					ESP_LOG_CONFIG("WiFi connected to %s for %ums",
						known_wifis[0].ssid, kpiMeter.getResultInMs());

					return;
				}

				delay(1);
			}
		}
	}
}

wl_status_t ESP32NetworkController::getWirelessStatus()
{
	return WiFi.status();
}

ESP32NetworkController::ESP32NetworkController()
{
	esp_log("Default constructor");
}

ESP32NetworkController::~ESP32NetworkController()
{
	esp_log("Default destructor");
}
