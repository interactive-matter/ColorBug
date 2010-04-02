/*
 * delay.c
 *
 *  Created on: 26.08.2009
 *      Author: marcus
 */
#include "delay.h"

#include <util/delay.h>

void delay_ms(uint16_t ms) {

  for (uint16_t i=ms;i>0;i--) {
    _delay_us(990);
  }

}
