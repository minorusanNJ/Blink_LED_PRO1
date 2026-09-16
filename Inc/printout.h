//main.c

#include "system.h"
#include "GPIO/gpio.h"
#include "bsp.h"
#include <stdio.h>
//Git practice
int main(void)
{
	system_init();
	bsp_init();

	while(1)
	{
		bsp_led_toggle();
		system_delay(800000);
		printf("LED toggled \r\n");
	}
}

//system.h

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f4xx.h"
#include <stdint.h>

void system_init(void);
void system_delay(volatile uint32_t count);

#endif

//system.c

#include "system.h"

void system_init(void)
{
	;
}

void system_delay(volatile uint32_t count)
{
	while(count--)
	{
		__NOP();
	}
}

//gpio.h

#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f4xx.h"
#include <stdint.h>

typedef enum
{
	LOW 	= 	0,
	HIGH	=	1
}gpio_pin_state_t;

static void gpio_clock_enable(GPIO_TypeDef *GPIOx);

void gpio_init_output(GPIO_TypeDef *GPIOx, uint8_t pin);
void gpio_set(GPIO_TypeDef *GPIOx, uint8_t pin);
void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t pin);
void gpio_write(GPIO_TypeDef *GPIOx, uint8_t pin, gpio_pin_state_t state);
void gpio_toggle(GPIO_TypeDef *GPIOx, uint8_t pin);

#endif

//gpio.c

#include "gpio.h"
//ドライバーは汎用性をもたらすことが大事。
//また、定義設定を変えれば他のボードで使える。
//CMSISの構造体定義を使っている。
//RCC	AHB1ENR
#define	GPIOAEN			(1U << 0)
#define	GPIOBEN			(1U << 1)
#define	GPIOCEN			(1U << 2)
#define	GPIODEN			(1U << 3)
//GPIO
#define	MODER_PIN_RST 	~(3U << (pin *2))
#define MODER_PIN_OUT	(1U << (pin *2))
#define	BSRR_PIN_H		(1U << pin)
#define BSRR_PIN_L		(1U << (pin + 16))
#define ODR_PIN_H		(1U << pin)

static void gpio_clock_enable(GPIO_TypeDef *GPIOx)
{
	if(GPIOx == GPIOA)
	{
		RCC->AHB1ENR	|=	GPIOAEN;
	}
	if(GPIOx == GPIOB)
	{
		RCC->AHB1ENR	|=	GPIOBEN;
	}
	if(GPIOx == GPIOC)
	{
		RCC->AHB1ENR	|=	GPIOCEN;
	}
	if(GPIOx == GPIOD)
	{
		RCC->AHB1ENR	|=	GPIODEN;
	}
}

void gpio_init_output(GPIO_TypeDef *GPIOx, uint8_t pin)
{
	gpio_clock_enable(GPIOx);

	GPIOx->MODER	&=	MODER_PIN_RST;
	GPIOx->MODER	|=	MODER_PIN_OUT;
}

void gpio_set(GPIO_TypeDef *GPIOx, uint8_t pin)
{
	GPIOx->BSRR		|=	BSRR_PIN_H;
}

void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t pin)
{
	GPIOx->BSRR		|=	BSRR_PIN_L;
}

void gpio_write(GPIO_TypeDef *GPIOx, uint8_t pin, gpio_pin_state_t state)
{
	if(state == HIGH)
	{
		gpio_set(GPIOx, pin);
	}
	else
	{
		gpio_reset(GPIOx, pin);
	}
}

void gpio_toggle(GPIO_TypeDef *GPIOx, uint8_t pin)
{
	if (GPIOx->ODR & ODR_PIN_H)
	{
		gpio_reset(GPIOx, pin);
	}
	else
	{
		gpio_set(GPIOx, pin);
	}
}

//uart.h

#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx.h"
#include <stdint.h>

void uart_init(void);

#endif

//uart.c

#include "UART/uart.h"
//RCC APB1: PA2 USART2_TX AF07
#define USART2EN		(1U << 17)
//RCC AHB1
#define GPIOAEN			(1U << 0)
//GPIO
#define MODER_PA2_RST	~(3U << (2 * 2))
#define MODER_PA2_AF	(2U << (2 * 2))
#define AFRL_PA2_RST	~(0xF << (2 * 4))
#define AFRL_PA2_AF7	(7U << (2 * 4))
//UART
#define CR1_UE			(1U << 13)
#define CR1_TE			(1U << 3)
#define APB1_CLK		16000000U
#define TERMBAUD		115200U
//UART SR
#define SR_TXE			(1U << 7)


void uart_init(void)
{
	RCC->APB1ENR	|=	USART2EN;
	RCC->AHB1ENR	|=	GPIOAEN;

	GPIOA->MODER	&=	MODER_PA2_RST;
	GPIOA->MODER	|=	MODER_PA2_AF;
	GPIOA->AFR[0]	&=	AFRL_PA2_RST;
	GPIOA->AFR[0]	|=	AFRL_PA2_AF7;

	USART2->BRR		=	(uint32_t)((APB1_CLK / TERMBAUD) + 0.5);
	USART2->CR1		|=	(CR1_UE | CR1_TE);
}

void uart_tx_write(int ch)
{
	while(!(USART2->SR & SR_TXE)){ }
	USART2->DR	=	(ch & 0xFF);
}

int __io_putchar(int ch)
{
	uart_tx_write(ch);
	return ch;
}

//bsp.h

#ifndef __BSP_H__
#define __BSP_H__

#include "bsp_led.h"

void bsp_init(void);

#endif

//bsp.c

#include "bsp.h"
#include "bsp_led.h"
#include "UART/uart.h"

//ボードで実行したいこと
void bsp_init(void)
{
	bsp_led_init();

	uart_init();
}

//bsp_led.h

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

//bsp_led.c

//Board specific information matter
#include "bsp_led.h"
//このボードで実行するので、ボード特定の定義を使っている。
//別のボードの場合、定義を変えるだけで使用できる。
#define	LED_PORT	GPIOA
#define	LED_PIN		5U

void bsp_led_init(void)
{
	gpio_init_output(LED_PORT, LED_PIN);
}

void bsp_led_on(void)
{
	gpio_set(LED_PORT, LED_PIN);
}

void bsp_led_off(void)
{
	gpio_reset(LED_PORT, LED_PIN);
}

void bsp_led_toggle(void)
{
	gpio_toggle(LED_PORT, LED_PIN);
}
