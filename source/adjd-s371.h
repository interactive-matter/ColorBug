/*
 * adjd-s371.h
 *
 *  Created on: 25.08.2009
 *      Author: marcus
 */

#ifndef ADJDS371_H_
#define ADJDS371_H_

#include <inttypes.h>

#define ADJDS371_GAIN_MAX = 4096
#define ADJDS371_CAP_MAX = 16

typedef struct _adjd_color_value {
  uint16_t clear;
  uint16_t red;
  uint16_t green;
  uint16_t blue;
} adjd_color_value;

typedef struct _adjd_int_time_value {
  uint16_t clear;
  uint16_t red;
  uint16_t green;
  uint16_t blue;
} adjd_int_time_value;

typedef struct _adjd_cap_value {
  uint8_t clear;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} adjd_cap_value;

#define ADJD_S371_OFF 0
#define ADJD_S371_ON 1

int adjd_s371_init (void);
//gain related
void adjd_s371_get_int_time(adjd_int_time_value* value);
uint8_t adjd_s371_set_int_time(adjd_int_time_value* value);
void adjd_s371_get_cap(adjd_cap_value* value);
uint8_t adjd_s371_set_cap(adjd_cap_value* value);
//color readout
uint8_t adjd_s371_enable_readout(uint8_t enable);
void adjd_s371_color_readout(adjd_color_value* value);
//built in light
uint8_t adjd_s371_enable_light(uint8_t enable);

#endif /* ADJDS371_H_ */
