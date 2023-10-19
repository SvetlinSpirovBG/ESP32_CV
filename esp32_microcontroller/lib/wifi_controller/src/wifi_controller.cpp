/**
 * @file wifi_controller.cpp
 *
 * @brief ESP32 WiFi controller
*/

#include "wifi_controller.h"

#include <Arduino.h>
#include "esp_http_client.h"

#include "esp_camera.h"
#include "esp_dev_lib.hpp"

// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_SEGMENT_SIZE 1436	//Maximum TCP segment size is 1,436 bytes
#define PACKET_SIZE 10
#define MAX_MESSAGE_SIZE (MAX_SEGMENT_SIZE - PACKET_SIZE)	//Maximum message size is 1,436 - PACKET_SIZE (header, footer, etc) bytes

#define WL_MAX_CONN_TRIES 2000

static volatile bool g_wifiConnected = false;

// List of ssid/password pair of known WiFi networks
static const wifi_connection_t known_wifis[] = {
	// {
	// 	{.ssid     = "Svetlin's iPhone"},
	// 	{.password = "batearni"},
	// },
	{
		{.ssid 	   = "TP-Link_37DD"},
		{.password = "Toni1998"},
	},
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
	ESP_LOG_CONFIG("ESP32 WiFi controller");

	TimestampMeter kpiMeter;
	size_t num_networks = sizeof(known_wifis) / sizeof(known_wifis[0]);

	ESP_LOG_INFO("Number of known networks = %d", num_networks);
	//Print networks for debug only
	for (size_t i = 0; i < num_networks; i++)
	{
		ESP_LOG_VERBOSE("  %s", known_wifis[i].ssid);
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
					ESP_LOG_INFO("Connecting to network %s...", known_wifis[j].ssid);

					for (int attempt = 0; attempt < max_conn_attemps; attempt++)
					{
						if (WiFi.status() == WL_CONNECTED)
						{
							ESP_LOG_CONFIG("WiFi connected to %s for %ums", known_wifis[j].ssid, kpiMeter.getResultInMs());

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

			ESP_LOG_INFO("Connecting to network %s...", known_wifis[0].ssid);

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

esp_err_t ESP32NetworkController::connectToHostSocket()
{
    int status, valread;
    struct sockaddr_in servAddr;
	char socketAddr[] = "192.168.0.106"; //VM Ubuntu's network adapter (ens33 in Svetlin's case)
    char payload[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi ac tellus lacinia, aliquet leo quis, tempus massa. Mauris tempus dolor tincidunt lorem mattis, quis convallis nibh malesuada. Curabitur suscipit sed arcu a consequat. Quisque vitae finibus lectus. Curabitur vestibulum leo in dolor feugiat, sed consectetur odio tempor. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Maecenas urna leo, hendrerit eget malesuada ut, eleifend at leo. Integer varius aliquam augue, non tempus nisi ullamcorper ac. Sed et hendrerit odio, ac posuere magna. Nunc tristique, nulla eu commodo sagittis, purus tortor cursus dolor, ac varius eros sem at turpis. In venenatis laoreet arcu ac malesuada. Donec eleifend hendrerit turpis nec laoreet. Integer luctus libero sem, sit amet pulvinar nisl sodales ac. Aenean sit amet sollicitudin arcu. Cras ac lacinia libero. Nam et sem vestibulum, condimentum metus eu, sodales dolor. Proin interdum quam vel sem blandit, ac convallis mi sodales. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Cras porta ultrices ultrices. Integer dapibus aliquet eros, et viverra nunc finibus sed. In a erat finibus, vulputate mi non, venenatis felis. In vitae nunc porttitor, lobortis eros ac, auctor magna. Donec fermentum porttitor diam eget iaculis. Sed erat elit, sagittis sed mattis sed, mollis gravida sapien. Na. Putka ti maychina v guza";
    char buffer[1024] = { 0 };

    ESP_LOG_CONFIG("Connecting to host socket");

	m_socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (m_socketFd < 0)
	{
        ESP_LOG_ERROR("Client socket creation error!");

        return ESP_FAIL;
    }

    ESP_LOG_VERBOSE("Client socket created");

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
	// @SV_WARN: This address is the one in Ubuntu VM (ens33)
	// https://www.youtube.com/watch?v=3bFcaTgUzfY
	// https://forum.arduino.cc/t/tcp-ip-socket-communication-esp32/1087260
    if (inet_pton(AF_INET, socketAddr, &servAddr.sin_addr) <= 0)
	{
        ESP_LOG_ERROR("Invalid address / Address not supported! %s", socketAddr);

        return ESP_FAIL;
    }

	status = connect(m_socketFd, (struct sockaddr*)&servAddr, sizeof(servAddr));

	if (status < 0)
	{
        ESP_LOG_ERROR("Failed to initiate a connection on socket address %s", socketAddr);

        return ESP_FAIL;
    }

    ESP_LOG_CONFIG("Successfully connected socket to host address %s", socketAddr);

	sendMessage((uint8_t*)payload, sizeof(payload) / sizeof(payload[0]));

    return ESP_OK;
	//Socket created. Sending and receiving messages should happen in dedicated functions.






    valread = read(m_socketFd, buffer, 1024);
    ESP_LOG_VERBOSE("%s\n", buffer);

    // closing the connected socket
    close(m_socketFd);

    return ESP_OK;
}

esp_err_t ESP32NetworkController::sendMessage(uint8_t* pPayload, size_t payloadSize)
{
	//@SV_TODO: If the payloadSize - Svetlin's header size is > MAX_SEGMENT_SIZE, then split it to few payloads of MSS size.
    uint8_t msg[MAX_SEGMENT_SIZE] = { 0 };
	size_t msgSize, segmentSize, remainingSize;
	TimestampMeter kpi;
	uint16_t msgSegments; //Number of message segments of. The payload will be cut in as many pieces fit in payloadSize(B) / MSS(B)
	uint8_t* pPositionIndicator;

	msgSegments = (uint16_t)std::ceil((float)payloadSize / MAX_SEGMENT_SIZE);

	ESP_LOG_VERBOSE("Sending %zuB message. Number of message segments: %d", payloadSize, msgSegments);

	if (m_socketFd < 0)
	{
		ESP_LOG_ERROR("Socket file descriptor is invalid! Cannot send message");
		return ESP_FAIL;
	}

	remainingSize = payloadSize;
	pPositionIndicator = pPayload;

	//@SV_TODO: Use while loop
	//Cut the payload into pieces of MAX_SEGMENT_SIZE and send msgSegments messages.
	for (uint16_t i = 0; i < msgSegments; i++, remainingSize -= MAX_MESSAGE_SIZE, pPositionIndicator += segmentSize)
	{
		// @SV_TODO: Check for overflow
		segmentSize = std::min((int)remainingSize, MAX_MESSAGE_SIZE);
		ESP_LOG_VERBOSE("Sending %d of %d message. Message size: %zu", i + 1, msgSegments, segmentSize);

		//Clear the msg buffer
		memset(msg, 0, MAX_SEGMENT_SIZE);

		//Start 6 byte sequence
		int j = 0;
		msg[j++] = 0xA1; 			//Custom message header 1
		msg[j++] = 0xB2; 			//Custom message header 2
		msg[j++] = 0xC3; 			//Custom message header 3
		msg[j++] = i + 1;			//Message index
		msg[j++] = msgSegments; 	//Total message segments
		msg[j++] = (segmentSize & 0xFF00) >> 8;		//Size (MSB)
		msg[j++] = segmentSize & 0xFF;		//Size (LSB)

		//From 7-th byte onward, copy the input payload
		memcpy(&msg[j], pPositionIndicator, segmentSize);

		//End byte sequence
		msg[j+segmentSize] = 0xF8;
		j++;
		msg[j+segmentSize] = 0xFA;
		j++;
		msg[j+segmentSize] = 0xFC;
		j++;

		msgSize = segmentSize + PACKET_SIZE;

		send(m_socketFd, msg, msgSize, MSG_DONTWAIT);
	}

	ESP_LOG_VERBOSE("Transmission message sent for %d ms", kpi.getResultInMs());

	return ESP_OK;
}

ESP32NetworkController::ESP32NetworkController()
{
}

ESP32NetworkController::~ESP32NetworkController()
{
}
