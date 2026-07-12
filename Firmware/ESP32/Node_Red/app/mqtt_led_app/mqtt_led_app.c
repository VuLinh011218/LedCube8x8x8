#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "driver/gpio.h"
#include "esp_crt_bundle.h"
#include "min.h"
#include "mqtt_led_app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_gpio.h"

#define TAG "MQTT_APP"

//==== MQTT Definitions ====
#define TOPIC_CMD        "linh/led/cmd"      // Receive: MODE_xxx
#define TOPIC_SPEED      "linh/led/speed"    // Receive: number (ms)
#define TOPIC_STATUS_ESP "linh/status/esp32" // Send: online/offline
#define TOPIC_STATUS_STM "linh/status/stm32" // Send: connected/error
#define TOPIC_CONFIRM    "linh/led/confirm"  // Send: on (activate Execution LED)

#define MQTT_DATA_BUF_SIZE 64
#define MIN_ID_CMD_STR     0x03 // The ID contains a string of commands (e.g., "MODE_RAIN")
#define MIN_ID_SPEED       0x04 // The ID contains the speed (e.g., 500)
#define MIN_ID_PONG        0x05

#define BROKER_URI "mqtts://19b8361271684e5f95c38dbcbf1d4990.s1.eu.hivemq.cloud:8883"
#define MQTT_USER  "linh_esp32"
#define MQTT_PASS  "Linh123456"

esp_mqtt_client_handle_t  g_mqtt_client = NULL;
extern struct min_context g_min_ctx;
extern void               min_port_set_mqtt_client(esp_mqtt_client_handle_t client);

// ---- MQTT Event Handler ----
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t  event  = (esp_mqtt_event_handle_t)event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id)
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected!");

            // 1. Subscribe to the control topics.
            esp_mqtt_client_subscribe(client, TOPIC_CMD, 0);
            esp_mqtt_client_subscribe(client, TOPIC_SPEED, 0);

            // 2. Online status report
            esp_mqtt_client_publish(client, TOPIC_STATUS_ESP, "online", 0, 1, 1);

            break;

        case MQTT_EVENT_DATA:
            char data_buf[MQTT_DATA_BUF_SIZE];

            int copy_len = event->data_len < (MQTT_DATA_BUF_SIZE - 1) ? event->data_len : (MQTT_DATA_BUF_SIZE - 1);
            memcpy(data_buf, event->data, copy_len);
            data_buf[copy_len] = '\0';

            // === CASE 1: Receive MODE (String) ===
            if (event->topic_len == strlen(TOPIC_CMD) && strncmp(event->topic, TOPIC_CMD, event->topic_len) == 0)
            {
                printf(">>> Forwarding CMD: %s -> STM32\n", data_buf);

                // Send the entire string to STM32
                min_send_frame(&g_min_ctx, MIN_ID_CMD_STR, (uint8_t *)data_buf, copy_len);
            }

            // === CASE 2: Receiving SPEED
            else if (event->topic_len == strlen(TOPIC_SPEED)
                     && strncmp(event->topic, TOPIC_SPEED, event->topic_len) == 0)
            {
                int      speed_int = atoi(data_buf);
                uint16_t speed_ms  = (uint16_t)speed_int;
                printf(">>> Forwarding SPEED: %d -> STM32\n", speed_ms);

                min_send_frame(&g_min_ctx, MIN_ID_SPEED, (uint8_t *)&speed_ms, sizeof(uint16_t));
            }
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT Error");
            break;

        default:
            break;
    }
}

// ---- Start Function ----
void mqtt_led_app_start(void)
{
    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = BROKER_URI,
        .broker.verification.crt_bundle_attach = esp_crt_bundle_attach,
        .credentials = {
            .username = MQTT_USER,
            .authentication = { .password = MQTT_PASS },
        },
        // --- LWT (Last Will) Configuration ---
        // When the connection is lost, the Broker will automatically change the status to "offline".
        .session.last_will = {
            .topic = TOPIC_STATUS_ESP,
            .msg = "offline",
            .qos = 1,
            .retain = 1
        }
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&cfg);

    // --- QUAN TRỌNG: TRUYỀN BIẾN VÀO MIN PORT ---
    min_port_set_mqtt_client(client);

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}
