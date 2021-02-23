#include "2560Map.h"
#include "I2CSlave.h"
#include "LEDs.h"
#include "Thrusters.h"
#include "Init.h"

/**
 * @brief Initialize stuff
 */
void initialization_sequence(){
  /* Init LED */
  initialize_leds();

  /* Init PWM */
  initialize_pwm();

  /* Enabling global interrupt */
  SREG = (1 << 7);

  /* Prearm thrusters */
  prearm_thruster_flood();

  // init I2C
  I2C_init(I2C_ADDR);

  #ifdef Arduino_h
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Initialization Complete...");
  #endif

  set_led_all(LED_ON);

}