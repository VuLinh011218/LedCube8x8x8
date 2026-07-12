idf_component_register(
    SRCS
        "uart_task.c"
       
    INCLUDE_DIRS
        "."

    REQUIRES esp_driver_uart driver min_port
)
