/*
 * lis302dl.h
 *
 *  Created on: 24.08.2009
 *      Author: marcus
 */

#ifndef LIS302DL_H_
#define LIS302DL_H_

#include <inttypes.h>


#define ACCEL_INT1_PORT PORTB
#define ACCEL_INT1_DDR DDRB
#define ACCEL_INT1_PIN PINB
#define ACCEL_INT1_POS _BV(0)

#define ACCEL_INT2_PORT PORTD
#define ACCEL_INT2_DDR DDRD
#define ACCEL_INT2_PIN PIND
#define ACCEL_INT2_POS _BV(7)

#define ACCEL_FDS_ENABLE _BV(4)
#define ACCEL_HP_FF_WU1 _BV(2)
#define ACCEL_HP_FF_WU2 _BV(3)

#define ACCEL_INT_NONE 0
#define ACCEL_INT_FF_WU_1 1
#define ACCEL_INT_FF_WU_2 2
#define ACCEL_INT_FF_WU_ANY 3
#define ACCEL_INT_DATA_RDY 4
#define ACCEL_INT_CLICK 7

typedef struct _accel_vect {
  uint8_t accel_x;
  uint8_t accel_y;
  uint8_t accel_z;
} accel_vect;

uint8_t lis302dl_init(void);
void lis302dl_selftest(uint8_t selftest);
void lis302dl_full_scale(uint8_t fullscale);
accel_vect lis302dl_read_accel(void);
void lis302dl_configure_filter(uint8_t filter_bits, uint8_t filter_coeff);
void lis302dl_configure_interrupt1(uint8_t interrupt_source);

#endif /* LIS302DL_H_ */
