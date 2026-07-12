#include <app_comms.h>
#include <effects_manager.h>
#include "ring_buffer.h"
#include "min.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#define MIN_ID_PONG    0x02
#define MIN_ID_CMD_STR 0x03
#define MIN_ID_SPEED   0x04

volatile CubeMode_t current_mode  = MODE_OFF;
volatile uint16_t   current_speed = 500;

extern UART_HandleTypeDef huart2;
extern void               UART_Process_TX(void);

static struct min_context min_ctx;
static uint8_t            rx_byte_u1;
static uint32_t           last_ping_tick = 0;
static uint8_t            is_connected   = 0;

uint8_t  tx_mem[512];
RingBuff tx_ring_buff;

void App_Comms_Init(void)
{
    ring_buff_init(&tx_ring_buff, tx_mem, 512);
    min_init_context(&min_ctx, 0);
    HAL_UART_Receive_IT(&huart2, &rx_byte_u1, 1);
}

// === Loop ===
void App_Comms_Run(void)
{
    if (is_connected && (HAL_GetTick() - last_ping_tick > 4000))
    {
        is_connected = 0;
    }
}

// === Callback ===
void min_application_handler(uint8_t min_id, uint8_t const *min_payload, uint8_t len_payload, uint8_t port)
{
    // --- MODE PROCESSING ---
    if (min_id == MIN_ID_CMD_STR)
    {
        char cmd_str[32];

        if (len_payload >= 32)
        {
            len_payload = 31;
        }
        memcpy(cmd_str, min_payload, len_payload);
        cmd_str[len_payload] = '\0'; // Kết thúc chuỗi

        // PING PROCESSING ---
        if (strncmp(cmd_str, "PING", 4) == 0)
        {
            // Cập nhật thời gian
            last_ping_tick = HAL_GetTick();
            is_connected   = 1;

            min_send_frame(&min_ctx, MIN_ID_PONG, (uint8_t *)"PONG", 4);
        }

        // --- SPEED PROCESSING ---
        else if (strncmp(cmd_str, "SPD:", 4) == 0)
        {
            int speed_val = atoi(cmd_str + 4);

            if (speed_val >= 10 && speed_val <= 1000)
            {
                current_speed = (uint16_t)speed_val;
            }
        }

        // Compare strings to set the Mode.
        else if (strcmp(cmd_str, "MODE_EDGE") == 0)
        {
            current_mode = MODE_EDGE;
        }
        else if (strcmp(cmd_str, "MODE_HEART") == 0)
        {
            current_mode = MODE_HEART;
        }
        else if (strcmp(cmd_str, "MODE_COUNT") == 0)
        {
            current_mode = MODE_COUNT;
        }
        else if (strcmp(cmd_str, "MODE_DIAGONAL") == 0)
        {
            current_mode = MODE_DIAGONAL;
        }
        else if (strcmp(cmd_str, "MODE_GROW") == 0)
        {
            current_mode = MODE_GROW;
        }
        else if (strcmp(cmd_str, "MODE_RAIN") == 0)
        {
            current_mode = MODE_RAIN;
        }
        else if (strcmp(cmd_str, "MODE_SPIRAL") == 0)
        {
            current_mode = MODE_SPIRAL;
        }
        else if (strcmp(cmd_str, "MODE_PLANE") == 0)
        {
            current_mode = MODE_PLANE;
        }
        else if (strcmp(cmd_str, "MODE_OFF") == 0)
        {
            current_mode = MODE_OFF;
        }
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        UART_Process_TX();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        min_poll(&min_ctx, &rx_byte_u1, 1);

        HAL_UART_Receive_IT(&huart2, &rx_byte_u1, 1);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);

        HAL_UART_Receive_IT(&huart2, &rx_byte_u1, 1);
    }
}
