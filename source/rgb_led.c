/*
 * rgb_led.c
 *
 *  Created on: 22.08.2009
 *      Author: marcus
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "status.h"
#include "delay.h"
#include "rgb_led.h"
#include "color.h"

#define SHD_DDR DDRD
#define SHD_PORT PORTD
#define SHD_PIN  _BV(3)

#define RGB_PORT PORTC
#define RGB_DDR  DDRC
#define RGB_PIN PINC
#define RED_PIN  _BV(0)
#define GREEN_PIN _BV(1)
#define BLUE_PIN _BV(2)

//self test delays
#define SINGLE_COLOR_DELAY 40
#define SWEEP_DELAY 5

//TODO mask for a faster RGB update
//TODO switch off if displaying 0,0,0

uint8_t red_value = 0;
uint8_t green_value = 0;
uint8_t blue_value = 0;
//internal buffer to ensure that the timer is always going to a complete circuit;
/*
uint8_t red_internal = 0;
uint8_t blue_internal = 0;
uint8_t green_internal = 0;
uint8_t rgb_counter = 0;
*/
register uint8_t red_internal asm("r2");
register uint8_t blue_internal asm("r3");
register uint8_t green_internal asm("r4");
register uint8_t rgb_counter asm("r5");
void
rgb_led_init(void)
{
  //shd is output & 1 for off
  SHD_DDR |= SHD_PIN;
  SHD_PORT |= SHD_PIN;
  //LED pins are outputs
  RGB_DDR |= RED_PIN | GREEN_PIN | BLUE_PIN;
  //self test
  //test red
  SHD_PORT &= ~SHD_PIN;
  RGB_PORT &= ~(RED_PIN | GREEN_PIN | BLUE_PIN);
  RGB_PORT |= RED_PIN;
  delay_ms(SINGLE_COLOR_DELAY);
  //test green
  RGB_PORT &= ~(RED_PIN | GREEN_PIN | BLUE_PIN);
  delay_ms(SINGLE_COLOR_DELAY);
  RGB_PORT |= GREEN_PIN;
  delay_ms(SINGLE_COLOR_DELAY);
  //test blue
  RGB_PORT &= ~(RED_PIN | GREEN_PIN | BLUE_PIN);
  delay_ms(SINGLE_COLOR_DELAY);
  RGB_PORT |= BLUE_PIN;
  delay_ms(SINGLE_COLOR_DELAY);
  RGB_PORT &= ~(RED_PIN | GREEN_PIN | BLUE_PIN);
  //display a color sweep
  /*rgb_led_enable();
   uint8_t i = 0;
   do {
   hsv_to_rgb(i,255,255,&red_value,&green_value,&blue_value);
   delay_ms(SWEEP_DELAY);
   i++;
   } while (i!=0);
   rgb_led_disable();*/
}

void
rgb_led_enable(void)
{
  _set_status(LED_IS_ON);
  SHD_PORT &= ~SHD_PIN;
  power_timer2_enable();
  rgb_counter = 0;
  //normal operation, just overflow
  TCCR2A = 0;
  TCCR2B = 1;
  //enable interrupts
  TIMSK2 |= _BV(TOIE2);
  sei();
}
void
rgb_led_disable(void)
{
  _clear_status(LED_IS_ON);
  SHD_PORT |= SHD_PIN;
  //stop timer
  TCCR2B = 0;
  //switch off the leds
  RGB_PORT &= ~(RED_PIN | GREEN_PIN | BLUE_PIN);
  //disable the timer interrupt
  TIMSK2 &= ~_BV(TOIE2);
  power_timer2_disable();
}

void
rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
  red_value = red;
  green_value = green;
  blue_value = blue;
}

ISR(TIMER2_OVF_vect)
{
/*  if (rgb_counter == 0)
    {
      red_internal = red_value;
      blue_internal = blue_value;
      green_internal = green_value;
      if (red_internal != 0)
        {
          RGB_PORT |= RED_PIN;
        }
      if (green_internal != 0)
        {
          RGB_PORT |= GREEN_PIN;
        }
      if (blue_internal != 0)
        {
          RGB_PORT |= BLUE_PIN;
        }
    }
  rgb_counter++;
  if (red_internal == rgb_counter)
    {
      RGB_PORT &= ~RED_PIN;
    }
  if (green_internal == rgb_counter)
    {
      RGB_PORT &= ~GREEN_PIN;
    }
  if (blue_internal == rgb_counter)
    {
      RGB_PORT &= ~BLUE_PIN;
    }*/
  //get the previous value and delete all RGB pins
    uint8_t rgb_value = RGB_PIN;
    if (rgb_counter == 0)
      {
        red_internal = red_value;
        blue_internal = blue_value;
        green_internal = green_value;
        rgb_value &=  ~(RED_PIN | GREEN_PIN | BLUE_PIN);
        rgb_value |= RED_PIN | GREEN_PIN | BLUE_PIN;
      }
        if (red_internal == rgb_counter)
          {
            rgb_value &= ~RED_PIN;
          }
        if (green_internal == rgb_counter)
          {
            rgb_value &= ~GREEN_PIN;
          }
        if (blue_internal == rgb_counter)
          {
            rgb_value &= ~BLUE_PIN;
          }
        RGB_PORT = rgb_value;
    rgb_counter++;
  }

