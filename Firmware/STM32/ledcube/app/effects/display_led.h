#ifndef __DISPLAY_LED_H
#define __DISPLAY_LED_H

#include "main.h"

#ifndef LATCH_PIN_Pin
#define LATCH_PIN_Pin       GPIO_PIN_4
#define LATCH_PIN_GPIO_Port GPIOA
#endif

void DisplayLed(uint8_t x, uint8_t y, uint8_t z);

#endif
