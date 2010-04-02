/*
 * status.h
 *
 *  Created on: 24.08.2009
 *      Author: marcus
 */

#ifndef STATUS_H_
#define STATUS_H_

#define LED_IS_ON 0

register uint8_t status asm("r7");

#define _set_status(bit) status |= _BV(bit)
#define _clear_status(bit) status &= ~_BV(bit)
#define _is_set(bit)  (status & _BV(bit))

#define HALT_LED TIMSK2 &= ~_BV(TOIE2)
#define ENABLE_LED TIMSK2 |= _BV(TOIE2)

#endif /* STATUS_H_ */
