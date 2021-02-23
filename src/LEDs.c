#include "2560Map.h"
#include "LEDs.h"

#include <avr/io.h>
#include <stdbool.h>


bool LED_D2_STATE = 0;
bool LED_D3_STATE = 0;

/**
 * @brief Initialize LEDs D2 and D3
 */
void initialize_leds()
{
	/* Clear port */
	LED_PORT = 0;

	/* Enable LEDs */
	LED_PORT |= (1 << LED_D2_PIN) | (1 << LED_D3_PIN);
}

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
void set_led(int LED_idx, bool on_off)
{
	/* Checking if valid LED */
	if (LED_idx < 0 || LED_idx > NUM_LEDs)
	{
		return;
	}

	switch (LED_idx % NUM_LEDs)
	{
	case LED_D2:
		if (on_off && !LED_D2_STATE) {
			LED_D2_STATE = 1;
			LED_PORT |= (1 << LED_D2_PIN);
		}
		else if (!on_off && LED_D2_STATE) {
			LED_D2_STATE = 0;
			LED_PORT &= !(1 << LED_D2_PIN);
		}
		break;

	case LED_D3:
		if (on_off && !LED_D3_STATE) {
			LED_D3_STATE = 1;
			LED_PORT |= (1 << LED_D3_PIN);
		}
		else if (!on_off && LED_D3_STATE) {
			LED_D3_STATE = 0;
			LED_PORT &= !(1 << LED_D3_PIN);
		}
		break;
	
	default:
		break;
	}
}

void set_led_all(bool on_off)
{
	for (int LED_idx = 0; LED_idx < NUM_LEDs; LED_idx++) {
		set_led(LED_idx, on_off);
	}
}