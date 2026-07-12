#ifndef __EDGE_EFFECT_H
#define __EDGE_EFFECT_H

#include "main.h"

void Effect_Around_Edge(int delay_ms, int iterations);

void Clear_Cube(void);
void Set_Voxel(uint8_t x, uint8_t y, uint8_t z, uint8_t state);
void Delay_Effect(uint32_t ms);

#endif
