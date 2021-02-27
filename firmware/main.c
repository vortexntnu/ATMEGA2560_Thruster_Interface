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

uint8_t num_received_bytes = 0;
uint16_t arming_counter = 0;

const uint8_t arming_array[NUM_THRUSTERS] = {ARM_UINT8};
bool is_armed = 0;

void I2C_received(uint8_t received_data);
void I2C_requested();

bool arming_check();
void set_thrusters();

int main()
{
	initialization_sequence();

	// Set received/requested callbacks for I2C
	I2C_setCallbacks(I2C_received, I2C_requested);

	// Main program loop
	while (1)
		;
}

void I2C_received(uint8_t received_data)
{
	recv_array[num_received_bytes] = received_data;
	num_received_bytes++;

	if (num_received_bytes == NUM_THRUSTERS)
	{
		if (arming_check)
			set_thrusters();
		num_received_bytes = 0;
	}
}

void I2C_requested()
{
	I2C_transmitByte(data);
}

bool arming_check()
{
	bool spin_thrusters = 0;
	if (recv_array[0] == ARM_UINT8 && recv_array[1] == ARM_UINT8 && recv_array[2] == ARM_UINT8 && recv_array[3] == ARM_UINT8 && recv_array[4] == ARM_UINT8 && recv_array[5] == ARM_UINT8 && recv_array[6] == ARM_UINT8 && recv_array[7] == ARM_UINT8)
	{
		if (!is_armed)
		{
			//Serial.println("ARMING");
			arming_counter++;
			if (arming_counter >= 1000)
			{
				arm_thrusters();
				is_armed = 1;
			}
		}
	}
	else if (!is_armed)
	{
		arming_counter = 0;
	}

	else
	{
		arming_counter = 0;
		spin_thrusters = 1;
	}

	return spin_thrusters;
}

void set_thrusters()
{
	for (int i = 0; i < NUM_THRUSTERS; i++)
		set_thrust(i, recv_array[i]);
}
