#ifndef __EFFECTS_MANAGER_H
#define __EFFECTS_MANAGER_H

#include "main.h"

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
	MODE_ADC,
	MODE_MAX
} CubeMode_t;

void Effects_Manager_Init(void);
void Effects_Manager_SetMode(CubeMode_t new_mode);
void Effects_Manager_Run(void);

#endif
