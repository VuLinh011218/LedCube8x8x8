#ifndef __COUNT_DOWN_CUBE_H
#define __COUNT_DOWN_CUBE_H

#include "main.h"

typedef enum {
	DIGIT_ZERO = 0,
	DIGIT_ONE,
	DIGIT_TWO, DIGIT_THREE, DIGIT_FOUR,
    DIGIT_FIVE, DIGIT_SIX, DIGIT_SEVEN, DIGIT_EIGHT, DIGIT_NINE
} digit_t;

void CountDownCube_Handle(void);
void CountDownCube_Set_State(void);

void Clear_Cube(void);
void Set_Voxel(uint8_t x, uint8_t y, uint8_t z, uint8_t state);
void Delay_Effect(uint32_t ms);

#endif
