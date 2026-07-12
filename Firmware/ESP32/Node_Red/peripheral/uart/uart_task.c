#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uart_task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "min.h"

#define RX_BUF_SIZE          256U
#define UART_DRIVER_BUF_SIZE 2048U
#define UART_PORT            UART_NUM_1
#define UART_TX              GPIO_NUM_17
#define UART_RX              GPIO_NUM_16

extern struct min_context g_min_ctx;

void uart_rx_task(void *arg)
{
    uint8_t *data = (uint8_t *)malloc(RX_BUF_SIZE);
    while (1)
    {
        int len = uart_read_bytes(UART_PORT, data, RX_BUF_SIZE, pdMS_TO_TICKS(20U));
        if (len > 0)
        {
            min_poll(&g_min_ctx, data, len);
        }
    }
    free(data);
}

void uart_init(void)
{
    uart_config_t cfg = {.baud_rate = UART_BAUD_115200,
                         .data_bits = UART_DATA_8_BITS,
                         .parity    = UART_PARITY_DISABLE,
                         .stop_bits = UART_STOP_BITS_1,
                         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    uart_driver_install(UART_PORT, UART_DRIVER_BUF_SIZE, 0, 0, NULL, 0); // queue_size=10
    uart_param_config(UART_PORT, &cfg);
    uart_set_pin(UART_PORT, UART_TX, UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_set_mode(UART_PORT, UART_MODE_UART);
}
