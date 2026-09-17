#include "system.h"

//FLASH
#define FLASH_LATENCY_RST		~(0xF << 0)
#define	FLASH_LATENCY_100		(3U << 0)
//PWR RCC APB1
#define APB1_PWREN				(1U << 28)
//PWR
#define	CR_VOS_100				(3U << 14)
//RCC
#define	CR_HSEON				(1U << 16)
#define	CR_HSERDY				(1U << 17)
#define	CR_PLLON				(1U << 24)
#define CR_PLLRDY				(1U << 25)
#define PLLCFGR_M_RST			~(0x3F << 0)
#define PLLCFGR_M_8				(8U << 0)
#define PLLCFGR_N_RST			~(0x1FF << 6)
#define PLLCFGR_N_200			(200U << 6)
#define	PLLCFGR_P_RST			~(3U << 16)
//#define PLLCFGR_P_4				(1U << 16)
#define	PLLCFGR_PLLSRC_HSEPLL	(1U << 22)
#define CFGR_PPRE2_RST			~(7U << 13)
#define CFGR_PPRE1_BY2			(4U << 10)
#define CFGR_HPRE_RST			~(0xF << 4)
#define CFGR_SW_PLL				(2U << 0)
#define CFGR_SWS_PLL			(2U << 2)
//SYSTICK
#define	CLKSRC					(1U << 2)
#define TICKINT					(1U << 1)
#define	ENABLE					(1U << 0)
#define CLK						100000000U

static void clk_100M_init(void)
{
	FLASH->ACR		&=	FLASH_LATENCY_RST;
	FLASH->ACR		|=	FLASH_LATENCY_100;

	RCC->CR			|=	CR_HSEON;
	while(!(RCC->CR & CR_HSERDY)){ }

	RCC->APB1ENR	|=	APB1_PWREN;
	PWR->CR			|=	CR_VOS_100;
	RCC->PLLCFGR	&=	(PLLCFGR_M_RST & PLLCFGR_N_RST & PLLCFGR_P_RST);
	RCC->PLLCFGR	|=	(PLLCFGR_M_8 | PLLCFGR_N_200 | PLLCFGR_PLLSRC_HSEPLL);
	RCC->CFGR		&=	(CFGR_PPRE2_RST & CFGR_HPRE_RST);
	RCC->CFGR		|=	CFGR_PPRE1_BY2;
	RCC->CR			|=	CR_PLLON;
	while(!(RCC->CR & CR_PLLRDY)){ }

	RCC->CFGR		|=	CFGR_SW_PLL;
	while(!(RCC->CFGR & CFGR_SWS_PLL)){ }
}


void system_init(void)
{
	clk_100M_init();
}

void systick_1ms_timer_init(void)
{
	SysTick->LOAD	=	(CLK * 0.001) - 1;
	SysTick->VAL	=	0;
	SysTick->CTRL	|=	(CLKSRC | TICKINT | ENABLE);
	NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void)
{
	ms_ticks++;
	if(ms_ticks >= ms_delay)
	{
		ms_ticks = 0;
		timer = 1;
	}
}


