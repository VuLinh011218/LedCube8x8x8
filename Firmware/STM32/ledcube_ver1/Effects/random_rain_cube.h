#ifndef __RANDOM_RAIN_CUBE_H
#define __RANDOM_RAIN_CUBE_H

#include "main.h"
#include <stdlib.h>

// Hàm điều khiển chính
void RandomRainCube_Handle(uint8_t rain_times);
void RandomRainCube_Set_State(void);

// Mượn hàm hiển thị từ driver của bạn
void DisplayLed(uint8_t x, uint8_t y, uint8_t z);

#endif
