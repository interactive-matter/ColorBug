/*
 * main.c
 *
 *  Created on: 22.08.2009
 *      Author: marcus
 */

#include <avr/power.h>
#include "delay.h"
#include "rgb_led.h"
#include  "lis302dl.h"
#include "accelerator.h"
#include "adjd-s371.h"

int
main(void)
{
	//switch anything off
	power_all_disable();
	//switch anything to input & disable pull up
	DDRB=0;
	PORTB=0;
	DDRC=0;
	PORTC=0;
	DDRD=0;
	PORTD=0;
	//lights on!
	rgb_led_init();
	accelerator_init();
	adjd_s371_init();

	rgb_led_enable();
	for(;;) {
	  accel_vect vec = lis302dl_read_accel();
	  rgb_led_set_color(vec.accel_x,vec.accel_y,vec.accel_z);
	  delay_ms(100);
	}
}
