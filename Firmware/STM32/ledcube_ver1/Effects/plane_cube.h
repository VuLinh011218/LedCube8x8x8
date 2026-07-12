#ifndef __PLANE_CUBE_H
#define __PLANE_CUBE_H

#include "main.h"

typedef enum {
    OX = 0,
    OY,
    OZ
} Plane_way_t;

void PlaneCube_Handle(void);
void PlaneCube_Set_State(void);

// Mượn các hàm từ main.c
void Clear_Cube(void);
void Set_Voxel(uint8_t x, uint8_t y, uint8_t z, uint8_t state);

#endif
