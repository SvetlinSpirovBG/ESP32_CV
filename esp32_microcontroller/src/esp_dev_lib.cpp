/**
 * @file esp_dev_lib.h
 *
 * @brief ESP32 Developer library.
 *
 * Implementation for commonly used functions, functions to develop & debug
*/

#include "esp32-hal.h"
#include "esp_dev_lib.hpp"

/**
 * @brief Setup the ESP32-CAM's built-in LED
 *
 * @param durationInMs	GPIO pin of LED
 *
 * @return None
*/
void setupLedFlash(int pin)
{
    #if CONFIG_LED_ILLUMINATOR_ENABLED
    ledcSetup(LED_LEDC_CHANNEL, 5000, 8);
    ledcAttachPin(pin, LED_LEDC_CHANNEL);
    #else
    esp_log("LED flash is disabled -> CONFIG_LED_ILLUMINATOR_ENABLED = 0");
    #endif
}

/**
 * @brief Blink the ESP32-CAM's LED N times for duration T (ms)
 *
 * @param durationInMs	Duration of each blink in milliseconds
 * @param numOfBlinks	Total number of LED blinks
 *
 * @return None
*/
void blinkLED(int durationInMs, int numOfBlinks)
{
	int max_led_intensity = 50; //@todo Add this as an input parameter
	for (int i = 0; i < numOfBlinks; i++) {
		// Turn on the flash
	    esp_log("Fire LED flash!");

		for (int i = 0; i < max_led_intensity; i++) {
			ledcWrite(2, i);
			delay(5);
		}

		// With LED on, wait for the LED flash duration
		delay(durationInMs);

		// Turn off the flash
		for (int i = max_led_intensity; i >= 0; i--) {
			ledcWrite(2, i);
			delay(10);
		}

		// Wait 1 second after a blink cycle
		// @todo Add this as an input parameter
		delay(durationInMs*2);
	}
}

TimestampMeter::TimestampMeter(/* args */)
{
	bootTimeStart = esp_timer_get_time();
	bootTimeEnd = 0;
}

TimestampMeter::~TimestampMeter()
{
}

//@SV_TODO: Make it a template to get result in float or in int?
int TimestampMeter::getResultInMs()
{
    bootTimeEnd = esp_timer_get_time();

    return (int)((bootTimeEnd - bootTimeStart) / 1000.0);
}
