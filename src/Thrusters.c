#include "2560Map.h"
#include "Thrusters.h"
#include "LEDs.h"
#include "Clamp.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

/**
 * @brief This function initializes the pwm
 * 
 * @warning Must be enabled before the thrusters
 */
/**
 * @brief Prototype to higher lever code to set register-values
 * 
 * @warning This code is only a prototype. It must be specified later
 * where this should be done, as the MCU can only handle 8 bits of code
 * 
 * @param thrust_idx Idx describing what thruster we would like to
 * write the command to. It is assumed that @p thrust_idx \in [0, 7]
 * 
 * @param thrust_val Value to be written to the thruster. It is assumed
 * that @p thrust_val \in [1100, 1900]. If the value is outside of this
 * range, the thruster is set to idle 
 */

void initialize_pwm()
{
	/* Clear ports */
	DDRE = 0;
	DDRH = 0;
	DDRL = 0;

	/* Setting ports as output */
	DDRE |= (1 << PE3) | (1 << PE4) | (1 << PE5);
	DDRH |= (1 << PH1) | (1 << PH3) | (1 << PH4) | (1 << PH5);
	DDRL |= (1 << PL3) | (1 << PL4);

	/* Initializing ports as pull-down */
	PORTE &= !((1 << PE3) | (1 << PE4) | (1 << PE5));
	PORTH &= !((1 << PH2) | (1 << PH1) | (1 << PH0));
	PORTL &= !((1 << PL3) | (1 << PL4));

	/* Setting A to Fast PWM, clearing OCnA and OCnB on compare match */
	/* | COMnA1 | COMnA0 | COMnB1 | COMnB0 | WGMn3 | WMGn2 | WMGn1 | WMGn0 | = | 1 | 0 | 1 | 0 | 0 | 1 | 1 | 0 | */
	TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM32) | (1 << WGM31);
	TCCR4A = (1 << COM4A1) | (1 << COM4B1) | (1 << COM4C1) | (1 << WGM42) | (1 << WGM41);
	TCCR5A = (1 << COM5A1) | (1 << COM5B1) | (1 << COM5C1) | (1 << WGM52) | (1 << WGM51);

	/* Setting clock-source to 1/64 * clk. See table 17-6 */
	TCCR3B = (1 << CS31) | (1 << CS30);
	TCCR4B = (1 << CS41) | (1 << CS40);
	TCCR5B = (1 << CS51) | (1 << CS50);

	/* Init timers */
	TCNT3 = 0;
	TCNT4 = 0;
	TCNT5 = 0;
}

void set_thrust(uint8_t thrust_idx, uint8_t thrust)
{
	/* Checking if valid thruster */
	if (thrust_idx < 0 || thrust_idx > NUM_THRUSTERS)
	{
		return;
	}

	uint8_t thrust_clamped = clamp(thrust, THRUST_FULL_REVERSE, THRUST_FULL_FORWARD);

	/* Switching between the correct thruster */
	switch (thrust_idx)
	{
	case 0:
		THR0 = thrust_clamped;
		break;

	case 1:
		THR1 = thrust_clamped;
		break;

	case 2:
		THR2 = thrust_clamped;
		break;

	case 3:
		THR3 = thrust_clamped;
		break;

	case 4:
		THR4 = thrust_clamped;
		break;

	case 5:
		THR5 = thrust_clamped;
		break;

	case 6:
		THR6 = thrust_clamped;
		break;

	case 7:
		THR7 = thrust_clamped;
		break;

	default:
		break;
	}
}

void set_thrust_all(uint8_t thrust)
{
    for (uint8_t i = 0; i < NUM_THRUSTERS; i++) set_thrust(i, thrust);
}

void prearm_thruster_flood()
{
	/* Turning LED D3 high and LED D2 low */
	set_led_all(LED_ON);

	/* Init thrusters to idle */
	set_thrust_all(200);

	/* Giving the thrusters 3 seconds to init */
	_delay_ms(100);
}

/**
 * @brief Function to initialize the thrusters to IDLE
 * 
 * @warning Must be enabled after the pwm
 * 
 * @warning The function assumes that the LED is initialized
 * 
 * @warning Must be set in at least a couple of seconds
 */
void arm_thrusters()
{
	/* Turning LED D3 high and LED D2 low */
	set_led(LED_D2, LED_ON);
	set_led(LED_D3, LED_OFF);

	/* Init thrusters to idle */
	set_thrust_all(THRUST_IDLE);

	/* Giving the thrusters 2 seconds to init */
	_delay_ms(2000);

	/* Turning LED D2 high and LED D3 low */
	set_led(LED_D2, !LED_D2_STATE);
	set_led(LED_D3, !LED_D3_STATE);

	/* Waiting for another second */
	_delay_ms(1000);
}