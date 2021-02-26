#include "Clamp.h"

#include <stdint.h>

uint8_t clamp(uint8_t value, uint8_t min, uint8_t max)
{
  return (value < min)? min : (value > max)? max : value;
}
