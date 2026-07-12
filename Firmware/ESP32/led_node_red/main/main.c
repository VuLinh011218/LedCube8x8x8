#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "connect_scan_wifi.h"
#include "mqtt_led_app.h"

#define TAG "MAIN_APP"
// ---- app_main ----
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

    mqtt_led_app_start();
}
