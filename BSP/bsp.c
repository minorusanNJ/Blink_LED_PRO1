#include "bsp.h"
#include "bsp_led.h"
//ボードで実行したいこと
void bsp_init(void)
{
	bsp_led_init();
	//bsp_led_on();
}
