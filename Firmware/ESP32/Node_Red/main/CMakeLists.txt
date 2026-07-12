idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES esp_event esp_netif nvs_flash mqtt driver connect_scan_wifi mqtt_led_app uart
    PRIV_REQUIRES mbedtls esp-tls min_port
)

