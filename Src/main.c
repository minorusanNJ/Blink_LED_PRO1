#include "system.h"
#include "bsp.h"
#include <stdio.h>
#include "data.h"

//Git practice
int main(void)
{
	system_init();
	bsp_init();
	systick_1ms_timer_init();
	ms_delay = 500;	//SysTickタイマー待ち時間㎳　毎にtimerのフラグが立つ

	while(1)
	{
		if(timer)
		{
			timer = 0;
			bsp_led_toggle();
			printf("LED toggled \r\n");
		}
	}
}


