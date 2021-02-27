#ifndef MAP2560_H
#define MAP2560_H

#include <avr/io.h>

/* ATMEGA2560 I2C Address. */
#define I2C_ADDR 0x8

/* Number of thrusters */
#define NUM_THRUSTERS 8
#define ARMING_COMMAND 69

/* Counters. Write the thrust values to these registers */
#define THR0 OCR3A /* Counter THR0                         */
#define THR1 OCR3B /* Counter THR1                         */
#define THR2 OCR3C /* Counter THR2                         */
#define THR3 OCR4A /* Counter THR3                         */
#define THR4 OCR4B /* Counter THR4                         */
#define THR5 OCR4C /* Counter THR5                         */
#define THR6 OCR5A /* Counter THR6                         */
#define THR7 OCR5B /* Counter THR7                         */

/* Thrust-values */
#define THRUST_FULL_REVERSE 140 /* Value that gives max reverse thrust  */
#define THRUST_IDLE 191			/* Value that gives idle thrust         */
#define THRUST_FULL_FORWARD 242 /* Value that gives max forward thrust  */
#define THRUST_MESSAGE_INIT 30   /* Value that declares the start of a thrust msg */
#define THRUST_MESSAGE_END 40    /* Value that declares the end of a thrust msg */

/* LEDS */
#define NUM_LEDs 2 /* The ports that the LEDS belong to    */
#define LED_PORT PORTF /* The ports that the LEDS belong to    */
#define LED_D2_PIN PF0 /* The pin that LED D2 belongs to       */
#define LED_D3_PIN PF1 /* The pin that LED D2 belongs to       */
#define LED_D2 0	   /* Idx of LED D2 on LED_PORT            */
#define LED_D3 1	   /* Idx of LED D3 on LED_PORT            */
#define LED_OFF 0	   /* Turn off the LED                     */
#define LED_ON 1	   /* Turn on the LED                      */

#endif /* MAP2560_H */
