#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"
#include <stdint.h>

typedef enum
{
	LOW 	= 	0,
	HIGH	=	1
}gpio_pin_state_t;

void gpio_clock_enable(GPIO_TypeDef *GPIOx);

void gpio_init_output(GPIO_TypeDef *GPIOx, uint8_t pin);
void gpio_set(GPIO_TypeDef *GPIOx, uint8_t pin);
void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t pin);
void gpio_write(GPIO_TypeDef *GPIOx, uint8_t pin, gpio_pin_state_t state);
void gpio_toggle(GPIO_TypeDef *GPIOx, uint8_t pin);

#endif
