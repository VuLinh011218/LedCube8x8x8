#include "min_port_esp32.h"
#include <string.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_client.h"
#include "min.h"
#include "ring_buffer.h"
#include "min_port_esp32.h"

#define TAG              "MIN_PORT"
#define TOPIC_STATUS_STM "linh/status/stm32" // Send: connected/error
#define MIN_UART_PORT    UART_NUM_1
#define MIN_ID_PONG      0x02

extern ring_buffer_t            g_rb;
extern struct min_context       g_min_ctx;
extern TaskHandle_t             g_led_task_handle;
static esp_mqtt_client_handle_t s_min_mqtt_client = NULL;

void min_port_set_mqtt_client(esp_mqtt_client_handle_t client)
{
    s_min_mqtt_client = client;
}

/* MIN will call this function to get the RX byte */
int min_transport_read(void)
{
    uint8_t b;
    if (rb_pop(&g_rb, &b))
    {
        return b;
    }
    else
    {
        return -1;
    }
}

void min_tx_byte(uint8_t port, uint8_t byte)
{
    uart_write_bytes(MIN_UART_PORT, &byte, sizeof(uint8_t));
}

uint32_t min_time_ms(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

uint16_t min_tx_space(uint8_t port)
{
    (void)port;
    return 64;
}

void min_tx_start(uint8_t port)
{
    (void)port;
}

void min_tx_finished(uint8_t port)
{
    (void)port;
}

/* --- MIN CALLBACK --- */
void min_application_handler(uint8_t min_id, uint8_t const *min_payload, uint8_t len_payload, uint8_t port)
{
    if (min_id == 1)
    {
        // 1. Print log frame and string
        char buffer[65];
        int  len = (len_payload < 64) ? len_payload : 64;
        memcpy(buffer, min_payload, len);
        buffer[len] = '\0';

        ESP_LOGI(TAG, "RECV FRAME -> ID: %d | Len: %d", min_id, len_payload);
        ESP_LOGI(TAG, "Message Content: %s", buffer);

        // 2. Send a signal to activate Task LED
        if (g_led_task_handle != NULL)
        {
            xTaskNotifyGive(g_led_task_handle);
        }
    }

    if (min_id == MIN_ID_PONG)
    {
        char resp_str[64];
        int  copy_len = (len_payload > 63) ? 63 : len_payload;
        memcpy(resp_str, min_payload, copy_len);
        resp_str[copy_len] = '\0';

        ESP_LOGI(TAG, "Recv STM32: %s", resp_str);

        if (s_min_mqtt_client != NULL)
        {
            esp_mqtt_client_publish(s_min_mqtt_client, TOPIC_STATUS_STM, resp_str, 0, 0, 0);
        }
    }
}
