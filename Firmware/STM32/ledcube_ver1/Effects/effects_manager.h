#ifndef __EFFECTS_MANAGER_H
#define __EFFECTS_MANAGER_H

#include "main.h"

// Định nghĩa 10 Mode tương ứng với giao diện Node-RED
typedef enum {
    MODE_OFF = 0,
    MODE_EDGE,
    MODE_HEART,
    MODE_COUNT,
    MODE_DIAGONAL,
    MODE_GROW,
    MODE_PLANE,
    MODE_RAIN,
    MODE_SPIRAL,
    MODE_ADC,        // Để trống theo yêu cầu
    MODE_MAX
} CubeMode_t;

// Các hàm quản lý
void Effects_Manager_Init(void);
void Effects_Manager_SetMode(CubeMode_t new_mode);
void Effects_Manager_Run(void);

#endif
