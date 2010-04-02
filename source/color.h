/*
 * color.h
 *
 *  Created on: 23.08.2009
 *      Author: marcus
 *
 *      taken from http://code.google.com/p/codalyze/wiki/CyzRgb
 */

#ifndef COLOR_H_
#define COLOR_H_

#import <inttypes.h>

#define MIN3(x,y,z)  ((y) <= (z) ? \
                         ((x) <= (y) ? (x) : (y)) \
                     : \
                         ((x) <= (z) ? (x) : (z)))

#define MAX3(x,y,z)  ((y) >= (z) ? \
                         ((x) >= (y) ? (x) : (y)) \
                     : \
                         ((x) >= (z) ? (x) : (z)))

void rgb_to_hsv(uint8_t red, uint8_t green, uint8_t bblue, uint8_t* hue, uint8_t* sat, uint8_t* val);

void hsv_to_rgb(uint8_t hue, uint8_t sat, uint8_t val, uint8_t* red, uint8_t* green, uint8_t* blue);


#endif /* COLOR_H_ */
