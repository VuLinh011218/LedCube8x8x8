#ifndef __DISPLAY_LED_H
#define __DISPLAY_LED_H

#include "main.h"

// Bạn hãy đảm bảo các Define này khớp với cấu hình trong ioc (CubeMX)
// Nếu chưa có, bạn có thể thay bằng GPIOA, GPIO_PIN_4 trực tiếp
#ifndef LATCH_PIN_Pin
#define LATCH_PIN_Pin       GPIO_PIN_4
#define LATCH_PIN_GPIO_Port GPIOA
#endif

// Hàm bật 1 điểm LED đơn lẻ tại tọa độ x, y, z
void DisplayLed(uint8_t x, uint8_t y, uint8_t z);

#endif
