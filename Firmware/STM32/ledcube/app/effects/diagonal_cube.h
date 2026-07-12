#ifndef __DIAGONAL_CUBE_H
#define __DIAGONAL_CUBE_H

#include "main.h"

typedef enum {
	SMALLEST = 0, MEDIUM, LARGE, LARGEST
} size_layer_t;

void DiagonalCube_Handle(void);
void DiagonalCube_Set_State(void);

void Clear_Cube(void);
void Set_Voxel(uint8_t x, uint8_t y, uint8_t z, uint8_t state);

#endif
