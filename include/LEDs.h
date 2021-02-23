#pragma once

#ifndef LEDs_H
#define LEDs_H

#include <stdbool.h>

extern bool LED_D2_STATE;
extern bool LED_D3_STATE;

void initialize_leds();
void set_led(int LED_idx, bool on_off);
void set_led_all(bool on_off);

#endif /* LEDs_H */