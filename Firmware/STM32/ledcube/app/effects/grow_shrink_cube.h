#ifndef __GROW_SHRINK_CUBE_H
#define __GROW_SHRINK_CUBE_H

#include "main.h"

typedef enum {
	TWO_X_TWO_CUBE = 0, FOUR_X_FOUR_CUBE, SIX_X_SIX_CUBE, EIGHT_X_EIGHT_CUBE
} SizeCube_t;

void GrowShrinkCube_Handle(void);
void GrowShrinkCube_Set_State(void);

void Clear_Cube(void);
void Set_Voxel(uint8_t x, uint8_t y, uint8_t z, uint8_t state);

#endif
