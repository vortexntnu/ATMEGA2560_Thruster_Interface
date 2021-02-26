#ifndef CLAMP_H
#define CLAMP_H

#include <stdint.h>

/**
 * @brief Returns clamped @p value between values @p min and @p max.
 * 
 * @param value Value to be clamped.
 * 
 * @param min If @p value is lower than @p min, @p min will be returned instead.
 * 
 * @param max If @p value is higher than @p max, @p max will be returned instead. 
 */
uint8_t clamp(uint8_t value, uint8_t min, uint8_t max);

#endif /* CLAMP_H */