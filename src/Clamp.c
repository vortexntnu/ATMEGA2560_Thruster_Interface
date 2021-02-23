#include "Clamp.h"

#include <stdint.h>

uint8_t clamp(uint8_t Value, uint8_t Min, uint8_t Max)
{
  return (Value < Min)? Min : (Value > Max)? Max : Value;
}
