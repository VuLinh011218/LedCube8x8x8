# File: main/CMakeLists.txt
# (Chỉ giữ lại đoạn này)

idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES esp_event esp_netif nvs_flash mqtt driver connect_scan_wifi mqtt_led_app
    PRIV_REQUIRES mbedtls esp-tls
)

