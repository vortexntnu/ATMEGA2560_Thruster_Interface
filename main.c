#include "2560Map.h"
#include "I2CSlave.h"
#include "LEDs.h"
#include "Thrusters.h"
#include "Init.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

#define ARM_UINT8 100

volatile char data;
volatile uint8_t recv_array[8];

uint8_t	num_received_bytes = 0;
uint16_t arming_counter = 0;

const uint8_t arming_array[NUM_THRUSTERS] = { ARM_UINT8 };
bool is_armed = 0;

void I2C_received(uint8_t received_data);
void I2C_requested();

void arming_check();
void set_thrusters();


int main()
{
	initialization_sequence();

	// Set received/requested callbacks for I2C
	I2C_setCallbacks(I2C_received, I2C_requested);

	// Main program loop
	while (1);
}


void I2C_received(uint8_t received_data)
{
	recv_array[num_received_bytes] = received_data;
	num_received_bytes++;

	if (num_received_bytes == NUM_THRUSTERS)
	{
		if (is_armed) set_thrusters();
		else arming_check();
		num_received_bytes = 0;
	}
}

void I2C_requested()
{
	I2C_transmitByte(data);
}

void arming_check()
{
	if (recv_array[0] == ARM_UINT8)
	{
		//Serial.println("ARMING");
		arming_counter++;
		if (arming_counter >= 1000)
		{
			arm_thrusters();
			is_armed = 1;
		}
	}
	else
	{
		arming_counter = 0;
	}
}

void set_thrusters()
{
	for (int i = 0; i < NUM_THRUSTERS; i++)
	{
		set_thrust(i, recv_array[i]);
	}
}