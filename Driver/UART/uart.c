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
