/*
 * lis302dl.c
 *
 *  Created on: 24.08.2009
 *      Author: marcus
 */
#include <avr/io.h>
#include <avr/power.h>

#include "lis302dl.h"
#include "i2cmaster.h"

//is least significant bit f the accelerometer (wired on board)
#define LIS302_SDO 1

//general I2C address of the Accelerometer
#define LIS302_I2C_ADDRESS (0x1c + LIS302_SDO)

//some register definitions
#define LIS302_WHO_AM_I 0x0F
#define WHO_AM_I_RESULT 0x3B
#define LIS302_CTRL_REG1 0x20
#define LIS302_CTRL_REG2 0x20
#define LIS302_X_OUT 0x29
#define LIS302_Y_OUT 0x2b
#define LIS302_Z_OUT 0x2d


uint8_t lis302_read_register(uint8_t register_id);
void lis302_write_register(uint8_t register_id, uint8_t value);

uint8_t lis302dl_init(void) {
  power_twi_enable();
  i2c_init();
  //configure ports
  ACCEL_INT1_DDR &= ~(ACCEL_INT1_POS);
  ACCEL_INT1_PORT &= ~(ACCEL_INT1_POS);

  ACCEL_INT2_DDR &= ~(ACCEL_INT2_POS);
  ACCEL_INT2_PORT &= ~(ACCEL_INT2_POS);

  uint8_t who_am_i = 0;
  who_am_i = lis302_read_register(LIS302_WHO_AM_I);
  if (WHO_AM_I_RESULT!=who_am_i) {
    return -1;
  }

  //set active & enable axis
  uint8_t ctrl_reg_value = _BV(6) | _BV(2) | _BV(1) | _BV(0);
  lis302_write_register(LIS302_CTRL_REG1,ctrl_reg_value);
  uint8_t reg_value = lis302_read_register(LIS302_CTRL_REG1);
  if (reg_value!=ctrl_reg_value) {
    return -1;
  } else {
    return 0;
  }
}

void lis302dl_selftest(uint8_t selftest) {
  uint8_t register_value = lis302_read_register(LIS302_CTRL_REG1);
  if (selftest) {
    register_value |= _BV(4) | _BV(3);
  } else {
    register_value &= ~(_BV(4) | _BV(3));
  }
}

void lis302dl_full_scale(uint8_t fullscale) {
  uint8_t register_value = lis302_read_register(LIS302_CTRL_REG1);
  if (fullscale) {
    register_value |= _BV(5);
  } else {
    register_value &= ~_BV(5);
  }
}

accel_vect lis302dl_read_accel(void) {
  accel_vect return_value;

  return_value.accel_x=lis302_read_register(LIS302_X_OUT);
  return_value.accel_y=lis302_read_register(LIS302_Y_OUT);
  return_value.accel_z=lis302_read_register(LIS302_Z_OUT);

  return return_value;
}

void lis302dl_configure_filter(uint8_t filter_bits, uint8_t filter_coeff) {

}

void lis302dl_configure_interrupt1(uint8_t interrupt_source) {

}

void lis302_write_register(uint8_t register_address, uint8_t value) {
  i2c_start_wait(I2C_7BIT_WRITE(LIS302_I2C_ADDRESS));
  i2c_write(register_address);
  i2c_write(value);
  i2c_stop();
}

uint8_t lis302_read_register(uint8_t register_address) {
  uint8_t result = -1;
  i2c_start_wait(I2C_7BIT_WRITE(LIS302_I2C_ADDRESS));
  i2c_write(register_address);
  i2c_rep_start(I2C_7BIT_READ(LIS302_I2C_ADDRESS));
  result = i2c_readNak();
  i2c_stop();
  return result;
}
