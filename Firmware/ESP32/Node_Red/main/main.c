/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#include "min.h"
#include "ring_buffer.h"
#include "uart_task.h"
#include "connect_scan_wifi.h"
#include "mqtt_led_app.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define TAG                "MAIN_APP"
#define UART_RX_STACK_SIZE 4096U
#define MIN_ID_PING        0x01
#define MIN_ID_PONG        0x02
#define MIN_ID_COMMAND     0x20

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

struct min_context g_min_ctx;
ring_buffer_t      g_rb;
QueueHandle_t      g_uart_queue;
TaskHandle_t       g_led_task_handle = NULL;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/

void app_main(void)
{
    // Init NVS + WiFi
    ESP_LOGI(TAG, "Startup");
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_scan_and_print();

    wifi_init_sta();
    uart_init();
    min_init_context(&g_min_ctx, 0U);

    mqtt_led_app_start();
    ESP_LOGI(TAG, "System Started. Waiting for STM32...");
    xTaskCreate(uart_rx_task, "uart_rx", UART_RX_STACK_SIZE, NULL, 10U, NULL);
}
