#include "2560Map.h"
#include "Thrusters.h"
#include "LEDs.h"
#include "Clamp.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

const uint8_t THRUSTER_OFFSET[NUM_THRUSTERS] = {0, 0, 0, 0, 0, 0, 0, 0};

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

	if (thrust < THRUST_FULL_REVERSE || thrust > THRUST_FULL_FORWARD) {
		return;
	}

	uint8_t thrust_offset = thrust +  THRUSTER_OFFSET[thrust_idx];	
	
	/* Switching between the correct thruster */
	switch (thrust_idx)
	{
	case 0:
		THR0 = thrust_offset;
		break;

	case 1:
		THR1 = thrust_offset;
		break;

	case 2:
		THR2 = thrust_offset;
		break;

	case 3:
		THR3 = thrust_offset;
		break;

	case 4:
		THR4 = thrust_offset;
		break;

	case 5:
		THR5 = thrust_offset;
		break;

	case 6:
		THR6 = thrust_offset;
		break;

	case 7:
		THR7 = thrust_offset;
		break;

	default:
		break;
	}
}

void set_thrust_all(uint8_t thrust)
{
    for (uint8_t i = 0; i < NUM_THRUSTERS; i++) set_thrust(i, thrust);
}

void prearm_thrusters()
{
	/* Turning LED D3 high and LED D2 low */
	set_led_all(LED_ON);

	/* Init thrusters to idle */
	set_thrust_all(200);

	/* Giving the thrusters 3 seconds to init */
	_delay_ms(100);
}

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
