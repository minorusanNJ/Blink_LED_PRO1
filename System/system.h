#ifndef SYSTEM_H
#define SYSTEM_H

#include "stm32f4xx.h"
#include <stdint.h>

void system_init(void);
void system_delay(volatile uint32_t count);

#endif
