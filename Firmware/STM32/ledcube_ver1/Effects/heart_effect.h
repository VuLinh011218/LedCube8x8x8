#ifndef __HEART_EFFECT_H
#define __HEART_EFFECT_H

#include "main.h"

// Các hàm nằm trong heart_effect.c (Cho main.c gọi)
void Effect_Heart_Final(int speed);

// Các hàm nằm trong main.c (Cho heart_effect.c mượn)
void Clear_Cube(void);
void Set_Voxel(uint8_t x, uint8_t y, uint8_t z, uint8_t state);
void Delay_Effect(uint32_t ms);

#endif
