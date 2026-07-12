#ifndef MIN_PORT_ESP32_H
#define MIN_PORT_ESP32_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_client.h"

#define MIN_PORT        0
#define MIN_MAX_PAYLOAD 64
#define MIN_USE_CRC     1
#define MIN_DEBUG_PRINT 0

extern TaskHandle_t g_led_task_handle;

void min_port_set_mqtt_client(esp_mqtt_client_handle_t client);
#endif
