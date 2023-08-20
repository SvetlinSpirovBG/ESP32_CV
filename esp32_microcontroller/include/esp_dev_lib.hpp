/**
 * @file esp_dev_lib.hpp
 *
 * @brief ESP32 Developer library.
 *
 * Constants.
 * Type definitions.
 * Commonly used functions, functions to develop & debug
*/

#ifndef _ESP_DEV_LIB_HPP_
#define _ESP_DEV_LIB_HPP_

#include "esp_timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// ESP develop/debug logging macro function to print filename, function and line number
#define esp_log(fmt, args...) printf("\033[0;33m%s %s():%d\033[0m " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##args);
#define ESP_LOG_CONFIG(fmt, args...) printf("\033[0;35m%s %s():%d\033[0m " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##args);

// Enable LED FLASH setting
#define CONFIG_LED_ILLUMINATOR_ENABLED 1

// LED FLASH setup
#if CONFIG_LED_ILLUMINATOR_ENABLED
    #define LED_LEDC_CHANNEL 2 //Using different ledc channel/timer than camera
    #define CONFIG_LED_MAX_INTENSITY 255
    static int  led_duty = 0;
    static bool isStreaming = false;
#endif

void startCameraServer();
void setupLedFlash(int pin);
void blinkLED(int durationInMs, int numOfBlinks);

/**
 * @brief Timestamp meter. Developer class to conveniently get the time difference between two moments in time and
 * return the time difference in milliseconds
*/
class TimestampMeter
{
private:
	int64_t bootTimeStart;
	int64_t bootTimeEnd;
public:
    int getResultInMs();
    TimestampMeter();
    ~TimestampMeter();
};

#endif /* _ESP_DEV_LIB_HPP_ */



// EXAMPLE FUNCTION DOCUMENTATION
/**
 * @brief Starts the web server
 *
 * Create an instance of HTTP server and allocate memory/resources for it
 * depending upon the specified configuration.
 *
 * Example usage:
 * @code{c}
 *
 * //Function for starting the webserver
 * httpd_handle_t start_webserver(void)
 * {
 *      // Generate default configuration
 *      httpd_config_t config = HTTPD_DEFAULT_CONFIG();
 *
 *      // Empty handle to http_server
 *      httpd_handle_t server = NULL;
 *
 *      // Start the httpd server
 *      if (httpd_start(&server, &config) == ESP_OK) {
 *          // Register URI handlers
 *          httpd_register_uri_handler(server, &uri_get);
 *          httpd_register_uri_handler(server, &uri_post);
 *      }
 *      // If server failed to start, handle will be NULL
 *      return server;
 * }
 *
 * @endcode
 *
 * @param[in]  config   Configuration for new instance of the server
 * @param[out] handle   Handle to newly created instance of the server. NULL on error
 * @return
 *  - ESP_OK    : Instance created successfully
 *  - ESP_ERR_INVALID_ARG      : Null argument(s)
 *  - ESP_ERR_HTTPD_ALLOC_MEM  : Failed to allocate memory for instance
 *  - ESP_ERR_HTTPD_TASK       : Failed to launch server task
 */
