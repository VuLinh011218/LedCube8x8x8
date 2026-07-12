idf_component_register(
    SRCS "mqtt_led_app.c"
    INCLUDE_DIRS "."
    REQUIRES esp_event mqtt driver esp-tls min_port
)

