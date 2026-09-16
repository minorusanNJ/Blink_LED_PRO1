#include "bsp.h"
#include "bsp_led.h"
#include "UART/uart.h"

//ボードで実行したいこと
void bsp_init(void)
{
	bsp_led_init();

	uart_init();
}
