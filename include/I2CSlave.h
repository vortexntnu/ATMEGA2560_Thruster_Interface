#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <avr/interrupt.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes I²C communication protocol.
 * 
 * @param address Adress to be set as device slave address.
 * Expected   
 */
void I2C_init(uint8_t address);

/**
 * @brief De-initializes I²C Communication.
 */
void I2C_stop(void);

/**
 * @brief Set callback functions to use during runtime with I²C communication.
 * 
 * @param recv Callback function on a receive event.
 *
 * @param req Callback function on a request event.
 */
void I2C_setCallbacks(void (*recv)(uint8_t), void (*req)());

/**
 * @brief Transmit bytes using back to Master device.
 * 
 * @param data Byte to transmit.
 */
inline void __attribute__((always_inline)) I2C_transmitByte(uint8_t data)
{
  TWDR = data;
}

/**
 * @brief Enable I²C interrupts.
 */
ISR(TWI_vect);

#ifdef __cplusplus
}
#endif

#endif
