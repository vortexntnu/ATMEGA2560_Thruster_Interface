/**
 * @file
 */

#ifndef THRUSTERS_H
#define THRUSTERS_H

#include <stdint.h>

/**
 * @brief This function initializes the pwm
 * 
 * @warning Must be enabled before the thrusters
 */
void initialize_pwm();

/**
 * @brief Prototype to higher lever code to set register-values - send PWM-signal to an ESC.
 *
 * @param thrust_idx Idx describing what thruster we would like to
 * write the command to. It is assumed that @p thrust_idx \in [0, NUM_THRUSTERS]
 * 
 * @param thrust Value to be written to the thruster. It is assumed
 * that @p thrust \in [THRUST_FULL_REVERSE, THRUST_FULL_FORWARD]. If the value is outside of this
 * range, the thrust value is constrained by @ref clamp(uint8_t, uint8_t, uint8_t) \in [min, max] function.
 */
void set_thrust(uint8_t thrust_idx, uint8_t thrust);

/**
 * @brief Prototype to higher lever code to set register-values - send PWM-signal to all ESCs.
 *
 * @param thrust Value to be written to the thruster. It is assumed
 * that @p thrust \in [THRUST_FULL_REVERSE, THRUST_FULL_FORWARD]. If the value is outside of this
 * range, the thrust value is constrained by @ref clamp(uint8_t, uint8_t, uint8_t) \in [min, max] function.
 *
 * @warning Assumes the pwm is enabled.
 */
void set_thrust_all(uint8_t thrust);

/**
 * @brief Set thrusters to non-idle states before arming, 
 * so as to not cause accidental arming of ESCs.
 */
void prearm_thrusters();

/**
 * @brief Function to arm thrusters and set them to IDLE.
 * 
 * @warning Must be enabled after the pwm
 * 
 * @warning The function assumes that the LED is initialized
 * 
 * @warning Must be set in at least a couple of seconds
 */
void arm_thrusters();

#endif /* THRUSTERS_H */