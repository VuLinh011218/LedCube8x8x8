/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

#include "connect_scan_wifi.h"
#include "driver/gpio.h"
#include "esp_rom_gpio.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define BLINK_GPIO             GPIO_NUM_2
#define ESP_WIFI_SSID          "Trang"
#define ESP_WIFI_PASS          "0976078330"
#define DEFAULT_SCAN_LIST_SIZE 8U

#define LED_TASK_STACK_SIZE 2048U
#define LED_BLINK_DELAY_MS  500U
#define TAG                 "wifi_station"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

static int                s_retry_num       = 0;
static TaskHandle_t       s_wifi_led_handle = NULL;
static EventGroupHandle_t s_wifi_event_group;
static wifi_ap_record_t   ap_info[DEFAULT_SCAN_LIST_SIZE]; // Declare a global array to avoid stack overflow

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = ESP_WIFI_SSID,
            .password = ESP_WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    ESP_ERROR_CHECK(esp_wifi_start()); // Only call start if Wi-Fi is not already started.

    ESP_LOGI(TAG, "wifi init start finished.");

    EventBits_t bits
        = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", ESP_WIFI_SSID, ESP_WIFI_PASS);
        xTaskCreate(wifi_blink_task, "LED", LED_TASK_STACK_SIZE, NULL, 10U, &s_wifi_led_handle);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Fail to connect to SSID:%s", ESP_WIFI_SSID);
    }
    else
    {
        ESP_LOGI(TAG, "UNEXPECTED EVENT");
    }

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    vEventGroupDelete(s_wifi_event_group);
}

void wifi_scan_and_print(void)
{
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    wifi_scan_config_t scan_config = {.ssid = 0, .bssid = 0, .channel = 0, .show_hidden = true};

    printf("\n Start WiFi scanning...\n");
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

    uint16_t ap_count = 0;
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    if (ap_count < number)
    {
        number = ap_count;
    }

    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));

    printf(" Scan completed, found %d WiFi:\n", ap_count);
    for (int i = 0; i < number; i++)
    {
        printf("%d: SSID: %s, RSSI: %d\n", i + 1, (char *)ap_info[i].ssid, ap_info[i].rssi);
    }
    ESP_ERROR_CHECK(esp_wifi_stop());
}

void wifi_blink_task(void *arg)
{
    esp_rom_gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(LED_BLINK_DELAY_MS));
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(LED_BLINK_DELAY_MS));
    }
}
/***********************************************************************************************************************
 * Private function
 **********************************************************************************************************************/

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < 10U)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}
