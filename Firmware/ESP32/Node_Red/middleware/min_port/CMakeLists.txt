get_filename_component(MIN_ABS_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../Third_Party/min/target" ABSOLUTE)

idf_component_register(
    SRCS 
        "min_port_esp32.c"
        "${MIN_ABS_PATH}/min.c"  
    INCLUDE_DIRS 
        "."
        "${MIN_ABS_PATH}"
    REQUIRES driver ring_buffer esp_timer mqtt
)