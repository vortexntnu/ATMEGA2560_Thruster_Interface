#include "2560Map.h"
#include "LEDs.h"

#include <avr/io.h>
#include <stdbool.h>

bool LED_D2_STATE = 0;
bool LED_D3_STATE = 0;

void initialize_leds()
{
	/* Clear port */
	LED_PORT = 0;

	/* Enable LEDs */
	LED_PORT |= (1 << LED_D2_PIN) | (1 << LED_D3_PIN);
}

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