#ifndef SYSTEM_H
#define SYSTEM_H

#include "stm32f4xx.h"
#include <stdint.h>
#include "data.h"

void system_init(void);
void systick_1ms_timer_init(void);
void systick_ms_delay(uint32_t count);

#endif
