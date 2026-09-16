#include "system.h"
#include "GPIO/gpio.h"
#include "bsp.h"

int main(void)
{
	system_init();
	bsp_init();

	while(1)
	{
		bsp_led_toggle();
		system_delay(800000);
	}
}
