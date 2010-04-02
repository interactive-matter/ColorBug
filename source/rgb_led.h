/*
 * rgb_led.h
 *
 *  Created on: 22.08.2009
 *      Author: marcus
 */

#ifndef RGB_LED_H_
#define RGB_LED_H_

#import <inttypes.h>

void
rgb_led_init(void);
void
rgb_led_enable(void);
void
rgb_led_disable(void);
void
rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue);

#endif /* RGB_LED_H_ */
