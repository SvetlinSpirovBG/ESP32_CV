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
//          foreground background
// black        30         40
// red          31         41
// green        32         42
// yellow       33         43
// blue         34         44
// magenta      35         45
// cyan         36         46
// white        37         47
//
// reset             0  (everything back to normal)
// bold/bright       1  (often a brighter shade of the same colour)
// underline         4
// inverse           7  (swap foreground and background colours)
// bold/bright off  21
// underline off    24
// inverse off      27
// "\033[1;31mbold red text\033[0m\n";

#define esp_log(fmt, args...) printf("\033[0;33m%s %s():%d\033[0m " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##args);
#define ESP_LOG_INFO(fmt, args...) printf("\033[0;33m%s %s():%d\033[0m " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##args);

#define ESP_LOG_VERBOSE(fmt, args...) printf("\033[7;30;47m%s %s():%d\033[0m " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##args);
#define ESP_LOG_CONFIG(fmt, args...) printf("\033[0;35m%s %s():%d\033[0m " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##args);
#define ESP_LOG_WARN(fmt, args...) printf("\033[7;33m;47%s %s():%d\033[0m " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##args);
#define ESP_LOG_ERROR(fmt, args...) printf("\033[7;31;47m%s %s():%d\033[0m " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##args);

// Enable LED FLASH setting
#define CONFIG_LED_ILLUMINATOR_ENABLED 1

// LED FLASH setup
#if CONFIG_LED_ILLUMINATOR_ENABLED
    #define LED_LEDC_CHANNEL 2 //Using different ledc channel/timer than camera
    #define CONFIG_LED_MAX_INTENSITY 255
    static int  led_duty = 0;
    static bool isStreaming = false;
#endif

//@SV_TODO: Move these function declarations to an appropriate file
void      startCameraServer();
void      setupLedFlash(int pin);
void      blinkLED(int durationInMs, int numOfBlinks);

/**
 * @brief Timestamp meter. Developer class to conveniently get the time difference between two moments in time and
 * return the time difference in milliseconds
*/
class TimestampMeter
{
private:
    /**
     * @brief Get time in microseconds since boot
     *
     * @return Number of microseconds since underlying timer has been started
     */
    int64_t getCurrentTime();

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
