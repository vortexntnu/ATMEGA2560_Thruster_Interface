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
volatile uint8_t recv_array[NUM_THRUSTERS];

uint8_t received_bytes_counter = 0;
uint16_t arming_counter = 0;

const uint8_t arming_array[NUM_THRUSTERS] = {ARM_UINT8};
bool is_armed = 0;

bool recv_mode = false;

void I2C_received(uint8_t received_byte);
void I2C_requested();

bool arming_check();
void set_thrusters();

int main()
{
	initialization_sequence();

	// Set received/requested callbacks for I2C
	I2C_setCallbacks(I2C_received, I2C_requested);

	// Main program loop
	while (1);
}

void I2C_received(uint8_t received_byte)
{
	if (!is_armed) {
		if (received_byte == ARMING_COMMAND) {
			arm_thrusters();
			is_armed = 1;
		}
	} 
	else if (received_byte == THRUST_MESSAGE_INIT) {
		recv_mode = true;
	}
	else if (received_byte == THRUST_MESSAGE_END) {
		set_thrusters(recv_array);
		for (int i=0; i<NUM_THRUSTERS; i++) {
			recv_array[i] = 0;
		}
		received_bytes_counter = 0;
		recv_mode = false;
	} 
	else if (recv_mode) {
		if (received_byte >= THRUST_FULL_REVERSE && received_byte <= THRUST_FULL_FORWARD) {
			recv_array[received_bytes_counter] = received_byte;
			received_bytes_counter++;
		}
	}
}

void I2C_requested()
{
	for (int i = 0; i < NUM_THRUSTERS; i++) {
		I2C_transmitByte(recv_array[i]);
	}
}

void set_thrusters(const uint8_t command_array[NUM_THRUSTERS])
{
	for (int i = 0; i < NUM_THRUSTERS; i++)
		set_thrust(i, command_array[i]);
}
