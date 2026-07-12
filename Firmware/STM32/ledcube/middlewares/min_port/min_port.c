#include "min.h"
#include "ring_buffer.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart2;
extern RingBuff           tx_ring_buff;

static uint8_t tx_byte_staging;

void UART_Process_TX(void)
{
    if (huart2.gState == HAL_UART_STATE_READY)
    {
        if (ring_buff_available(&tx_ring_buff) > 0)
        {
            ring_buff_pop(&tx_ring_buff, &tx_byte_staging);
            HAL_UART_Transmit_IT(&huart2, &tx_byte_staging, 1);
        }
    }
}

void min_tx_start(uint8_t port)
{
    (void)port;
}

void min_tx_byte(uint8_t port, uint8_t byte)
{
    (void)port;
    __disable_irq();
    ring_buff_push(&tx_ring_buff, byte);
    __enable_irq();
}

void min_tx_finished(uint8_t port)
{
    (void)port;
    UART_Process_TX();
}

uint16_t min_tx_space(uint8_t port)
{
    (void)port;
    return (tx_ring_buff.maxlen - ring_buff_available(&tx_ring_buff));
}

uint32_t min_time_ms(void)
{
    return HAL_GetTick();
}
