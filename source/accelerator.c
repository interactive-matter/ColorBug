/*
 * accelerator.c
 *
 *  Created on: 24.08.2009
 *      Author: marcus
 */
#include "delay.h"

#include <avr/sfr_defs.h>

#include "accelerator.h"
#include "lis302dl.h"
#include "rgb_led.h"

#define STATUS_DELAY 10

void accelerator_init(void) {
  rgb_led_set_color(255,255,0);
  rgb_led_enable();
  delay_ms(STATUS_DELAY);
  rgb_led_set_color(0,0,0);

  uint8_t status=lis302dl_init();
  if(status) {
    rgb_led_set_color(255,0,0);
    delay_ms(STATUS_DELAY*10);
  } else {
    rgb_led_set_color(0,255,0);
  }
  delay_ms(STATUS_DELAY);
  lis302dl_configure_filter(ACCEL_FDS_ENABLE,1);
  rgb_led_disable();
}
