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
