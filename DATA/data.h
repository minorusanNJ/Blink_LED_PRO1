#ifndef DATA_H
#define DATA_H

#include <stdint.h>
//SysTickの㎳カウンター
extern volatile uint32_t ms_ticks;
//SysTickの待ち時間
extern volatile uint32_t ms_delay;
//1msのフラグ１完了、０未完了
extern volatile uint8_t timer;

#endif
