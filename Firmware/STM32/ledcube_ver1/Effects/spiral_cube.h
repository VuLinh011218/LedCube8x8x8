#ifndef __SPIRAL_CUBE_H
#define __SPIRAL_CUBE_H

#include "main.h"

// Định nghĩa các kích thước vòng xoắn
typedef enum {
    ROLL_BIGGEST = 0,
    ROLL_BIGGER,
    ROLL_MEDIUM,
    ROLL_SMALL
} rollSize_t;

// Định nghĩa các trục di chuyển
typedef enum {
    Y_AXIS_1 = 0,
    X_AXIS_1,
    Y_AXIS_2,
    X_AXIS_2
} axis_t;

// Định nghĩa chiều quay
typedef enum {
    CLOCKWISE = 0,
    COUNTER_CLOCKWISE
} directRotate_t;

// Hàm điều khiển chính
void SpiralCube_Handle(void);
void SpiralCube_Set_State(void);

// Mượn hàm hiển thị và xóa cube
void DisplayLed(uint8_t x, uint8_t y, uint8_t z);
void Clear_Cube(void);

#endif
