#ifndef MQTT_H
#define MQTT_H
#include "mqtt_client.h"
#define TOPIC_LENGHT 40
void mqtt_event_handler(void *handler, esp_event_base_t base, int32_t event_id, void *event_data);
void start_mqtt();
void handle_mqtt_msg(esp_mqtt_event_handle_t event);
#endif
