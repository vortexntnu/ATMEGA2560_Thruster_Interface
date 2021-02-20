#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <avr/interrupt.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void I2C_init(uint8_t address);
void I2C_stop(void);
void I2C_setCallbacks(void (*recv)(uint8_t), void (*req)());

inline void __attribute__((always_inline)) I2C_transmitByte(uint8_t data)
{
  TWDR = data;
}

ISR(TWI_vect);

#ifdef __cplusplus
}
#endif

#endif
