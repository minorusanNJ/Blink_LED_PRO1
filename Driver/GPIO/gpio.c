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

void gpio_clock_enable(GPIO_TypeDef *GPIOx)
{
	if(GPIOx == GPIOA)
	{
		RCC->AHB1ENR	|=	GPIOAEN;
	}
	else if(GPIOx == GPIOB)
	{
		RCC->AHB1ENR	|=	GPIOBEN;
	}
	else if(GPIOx == GPIOC)
	{
		RCC->AHB1ENR	|=	GPIOCEN;
	}
	else if(GPIOx == GPIOD)
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
	GPIOx->BSRR		=	BSRR_PIN_H;		//BSRRは、＝で。
}

void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t pin)
{
	GPIOx->BSRR		=	BSRR_PIN_L;
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
