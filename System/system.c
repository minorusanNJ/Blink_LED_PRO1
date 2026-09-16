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
