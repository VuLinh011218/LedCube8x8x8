#include "min.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart2;

/* ================= TX ================= */

void min_tx_start(uint8_t port)
{
    (void)port;
    // STM32 UART blocking -> không cần làm gì
}

void min_tx_byte(uint8_t port, uint8_t byte)
{
    (void)port;
    HAL_UART_Transmit(&huart2, &byte, 1, HAL_MAX_DELAY);
}

void min_tx_finished(uint8_t port)
{
    (void)port;
    // STM32 UART blocking -> không cần làm gì
}

uint16_t min_tx_space(uint8_t port)
{
    (void)port;
    return 64;   // giả sử UART TX buffer đủ
}

/* ================= TIME ================= */

uint32_t min_time_ms(void)
{
    return HAL_GetTick();
}
