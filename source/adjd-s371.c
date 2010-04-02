/*
 * adjd-s371.c
 *
 *  Created on: 25.08.2009
 *      Author: marcus
 */
#include "adjd-s371.h"

#include <avr/io.h>
#include <avr/power.h>
#include "delay.h"
#include "i2cmaster.h"

#define LIGHT_PIN _BV(0)
#define LIGHT_PORT PORTD
#define LIGHT_DDR DDRD

//TODO move to color sensor
#define SLEEP1_PIN _BV(1)
#define SLEEP1_PORT PORTD
#define SLEEP1_DDR DDRD

#define SLEEP2_PIN _BV(2)
#define SLEEP2_PORT PORTD
#define SLEEP2_DDR DDRD

#define RESET_PIN _BV(0)
#define RESET_PORT PORTB
#define RESET_DDR DDRB
#define RESET_DELAY 2

#define ADJD_I2C_ADDRESS 0x74       // 7bit
#define REG_CTRL          0x00
#define REG_CONFIG        0x01

#define REG_CAP_RED       0x06
#define REG_CAP_GREEN     0x07
#define REG_CAP_BLUE      0x08
#define REG_CAP_CLEAR     0x09

#define REG_INT_RED_LO    0x0A
#define REG_INT_RED_HI    0x0B
#define REG_INT_GREEN_LO  0x0C
#define REG_INT_GREEN_HI  0x0D
#define REG_INT_BLUE_LO   0x0E
#define REG_INT_BLUE_HI   0x0F
#define REG_INT_CLEAR_LO  0x10
#define REG_INT_CLEAR_HI  0x11

#define REG_DATA_RED_LO   0x40
#define REG_DATA_RED_HI   0x41
#define REG_DATA_GREEN_LO 0x42
#define REG_DATA_GREEN_HI 0x43
#define REG_DATA_BLUE_LO  0x44
#define REG_DATA_BLUE_HI  0x45
#define REG_DATA_CLEAR_LO 0x46
#define REG_DATA_CLEAR_HI 0x47

#define INT_TIME_MAX 4095
#define CAP_MAX 15

#define LIGHT_DELAY 10

uint8_t
adjd_write_register(uint8_t register_address,uint8_t value);
uint8_t
adjd_read_register(uint8_t register_address);

int
adjd_s371_init(void)
{
  LIGHT_DDR |= LIGHT_PIN;
  LIGHT_PORT &= ~LIGHT_PIN;

  SLEEP1_DDR |= SLEEP1_PIN;
  SLEEP1_PORT &= ~SLEEP1_PIN;

  SLEEP2_DDR |= SLEEP2_PIN;
  SLEEP2_PORT &= ~SLEEP2_PIN;

  power_twi_enable();

  RESET_DDR |= RESET_PIN;
  RESET_PORT &= ~RESET_PIN;

  delay_ms(RESET_DELAY);
  RESET_PORT |= RESET_PIN;
  delay_ms(RESET_DELAY);

  i2c_init();

  uint8_t config_value = _BV(0);
  uint8_t comp = 0;

  SLEEP1_PORT &= ~SLEEP1_PIN;
  SLEEP2_PORT |= SLEEP2_PIN;
  delay_ms(RESET_DELAY);
  adjd_write_register(REG_CONFIG,config_value);
  comp = adjd_read_register(REG_CONFIG);
  if (comp!=config_value) {
    return -1;
  }

  SLEEP1_PORT |= SLEEP1_PIN;
   SLEEP2_PORT &= ~SLEEP2_PIN;
   delay_ms(RESET_DELAY);
   adjd_write_register(REG_CONFIG,config_value);
   comp = adjd_read_register(REG_CONFIG);
   if (comp!=config_value) {
     return -1;
   }

  adjd_s371_enable_light(1);
  delay_ms(LIGHT_DELAY);
  adjd_s371_enable_light(0);
  return 0;
}

//TODO error return values
void adjd_s371_get_int_time(adjd_int_time_value* value)
{
  //clip
  value->clear &= INT_TIME_MAX;
 value->red &= INT_TIME_MAX;
 value->green &= INT_TIME_MAX;
 value->blue &= INT_TIME_MAX;
 uint8_t hi;
 uint8_t lo;
 hi = value->clear >> 8;
 lo = value->clear;
 adjd_write_register(REG_INT_CLEAR_HI,hi);
 adjd_write_register(REG_INT_CLEAR_LO,hi);
 hi = value->red >> 8;
 lo = value->red;
 adjd_write_register(REG_INT_RED_HI,hi);
 adjd_write_register(REG_INT_RED_LO,hi);
 hi = value->green >> 8;
 lo = value->green;
 adjd_write_register(REG_INT_GREEN_HI,hi);
 adjd_write_register(REG_INT_GREEN_LO,hi);
 hi = value->blue >> 8;
 lo = value->blue;
 adjd_write_register(REG_INT_BLUE_HI,hi);
 adjd_write_register(REG_INT_BLUE_LO,hi);
}

uint8_t
adjd_s371_set_int_time(adjd_int_time_value* value)
{
  uint8_t hi;
  uint8_t lo;
  hi = adjd_read_register(REG_INT_CLEAR_HI);
  lo = adjd_read_register(REG_INT_CLEAR_LO);
  value->clear= hi << 8 | lo;
  hi = adjd_read_register(REG_INT_RED_HI);
  lo = adjd_read_register(REG_INT_RED_LO);
  value->red= hi << 8 | lo;
  hi = adjd_read_register(REG_INT_GREEN_HI);
  lo = adjd_read_register(REG_INT_GREEN_LO);
  value->green= hi << 8 | lo;
  hi = adjd_read_register(REG_INT_BLUE_HI);
  lo = adjd_read_register(REG_INT_BLUE_LO);
  value->blue= hi << 8 | lo;
  return 0;
}

void
adjd_s371_get_cap(adjd_cap_value* value)
{

}

uint8_t
adjd_s371_set_cap(adjd_cap_value* value)
{

}

uint8_t
adjd_s371_enable_readout(uint8_t enable)
{

}

void adjd_s371_color_readout(adjd_color_value* value)
{

}

uint8_t
adjd_s371_enable_light(uint8_t enable)
{
  if (enable)
    {
      LIGHT_PORT |= LIGHT_PIN;
    }
  else
    {
      LIGHT_PORT &= ~LIGHT_PIN;
    }
}

uint8_t adjd_write_register(uint8_t register_address,uint8_t value) {
  i2c_start_wait(I2C_7BIT_WRITE(ADJD_I2C_ADDRESS));
  i2c_write(register_address);
  i2c_write(value);
  i2c_stop();
}

uint8_t
adjd_read_register(uint8_t register_address)
{
  uint8_t result = -1;
  i2c_start_wait(I2C_7BIT_WRITE(ADJD_I2C_ADDRESS));
  i2c_write(register_address);
  i2c_rep_start(I2C_7BIT_READ(ADJD_I2C_ADDRESS));
  result = i2c_readNak();
  i2c_stop();
  return result;
}

