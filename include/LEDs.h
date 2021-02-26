#ifndef LEDs_H
#define LEDs_H

#include <stdbool.h>

/* LED state variables */
extern bool LED_D2_STATE;
extern bool LED_D3_STATE;

/**
 * @brief Initialize LEDs D2 and D3
 */
void initialize_leds();

/**
 * @brief Function to turn an LED on or off.
 * 
 * Use the macros to turn any LED on/off to ease the readability.
 * 
 * @warning The function assumes that the function initialize_leds have
 * been called.
 * 
 * @param LED_idx Index determining which LED is to be set.
 * 
 * If LED_idx == 0
 *    LED D2 will be set to @p on_off
 * 
 * If LED_idx == 1
 *    LED D3 will be set to @p on_off
 * 
 * @param on_off Boolean value describing if the LED is to be turned off or on.
 */
void set_led(int LED_idx, bool on_off);

/**
 * @brief Function to turn all LEDs on or off.
 * 
 * Use the macros to turn any LED on/off to ease the readability.
 * 
 * @warning The function assumes that the function initialize_leds have
 * been called.
 * 
 * @param on_off Boolean value describing if the LEDs is to be turned off or on.
 */
void set_led_all(bool on_off);

#endif /* LEDs_H */