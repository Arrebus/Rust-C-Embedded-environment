idf_component_register(SRCS "cJSON.c" "mqtt.c" "wifi.c" "main.c" "mqtt.c" "i2c.c"
                    INCLUDE_DIRS "../include"
                    REQUIRES mqtt esp_system esp_wifi esp_event nvs_flash lwip freertos driver
                    )