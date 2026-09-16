#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f4xx.h"
#include <stdint.h>
#include "GPIO/gpio.h"

void bsp_led_init(void);
void bsp_led_on(void);
void bsp_led_off(void);
void bsp_led_toggle(void);

#endif
