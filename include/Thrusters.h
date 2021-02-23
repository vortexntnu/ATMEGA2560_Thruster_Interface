#ifndef THRUSTERS_H
#define THRUSTERS_H

#include <stdint.h>

void initialize_pwm();
void set_thrust(uint8_t thrust_idx, uint8_t thrust);
void set_thrust_all(uint8_t thrust);
void prearm_thruster_flood();
void arm_thrusters();

#endif /* THRUSTERS_H */