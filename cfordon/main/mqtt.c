#include "mqtt_client.h"
#include <string.h>
#include <stdlib.h>
#include "mqtt.h"
#include "i2c.h"
#include <cJSON.h> 
// Hantering av event för mqtt
void mqtt_event_handler(void *handler, esp_event_base_t base, int32_t event_id, void *event_data){
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    
    switch((esp_mqtt_event_id_t)event_id){
        case MQTT_EVENT_CONNECTED:
            printf("MQTT CONNECTED...\n");
            msg_id = esp_mqtt_client_subscribe(client, "/user/setSpeed", 0);
            msg_id = esp_mqtt_client_subscribe(client, "/user/maxSpeed", 0);
            msg_id = esp_mqtt_client_subscribe(client, "/user/emergencyStopAll", 0);
            msg_id = esp_mqtt_client_subscribe(client, "/user/emergencyStop", 0);
            break;
        
        case MQTT_EVENT_PUBLISHED:
            printf("MQTT_EVENT_PUBLISHED, msg_id:%d\n", event->msg_id);
            break;

        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_DATA:
            handle_mqtt_msg(event);
            break;
        default:
        printf("Other event id:%d\n", event->event_id);
        break;
    }
}
    
void handle_mqtt_msg(esp_mqtt_event_handle_t event){

    int speed, carID;
    char* topic = malloc(event->topic_len + 1);
    strncpy(topic, event->topic, event->topic_len);
    topic[event->topic_len] = '\0';
    
    
    
    if(strcmp(topic, "/user/setSpeed") == 0){
        cJSON *json_data = cJSON_Parse(event->data);
        cJSON *speed_json = cJSON_GetObjectItemCaseSensitive(json_data, "speed");
        cJSON *carID_json = cJSON_GetObjectItemCaseSensitive(json_data, "carID");
        speed = atoi(speed_json->valuestring);
        carID = atoi(carID_json->valuestring);
        if(carID == VEHICLE_ID){set_speed(speed);}
    }
    if(strcmp(topic, "/user/maxSpeed") == 0){
        cJSON *json_data = cJSON_Parse(event->data);
        cJSON *max_speed_json = cJSON_GetObjectItemCaseSensitive(json_data, "max");
        cJSON *carID_json = cJSON_GetObjectItemCaseSensitive(json_data, "carID");
        speed = atoi(max_speed_json->valuestring);
        carID = atoi(carID_json->valuestring);
        if(carID == VEHICLE_ID){set_max_speed(speed);}
    }
    if(strcmp(topic, "/user/emergencyStopAll") == 0){
        bool state;
        char* state_string = malloc(event->data_len + 1);
        strncpy(state_string, event->data, event->data_len);
        state_string[event->data_len] = '\0';
        if(strcmp(state_string, "true") == 0){ state = true;}
        else{state = false;}
        set_emergency_stop(state);
    }
    if(strcmp(topic, "/user/emergencyStop") == 0){
        cJSON *json_data = cJSON_Parse(event->data);
        cJSON *json_state = cJSON_GetObjectItemCaseSensitive(json_data, "state");
        cJSON *carID_json = cJSON_GetObjectItemCaseSensitive(json_data, "carID");
        bool state = cJSON_IsTrue(json_state);
        carID = atoi(carID_json->valuestring);
        if(carID == VEHICLE_ID){set_emergency_stop(state);}
    }
    free(topic);
}
void start_mqtt(){
    //Configurerar mqtt
    esp_mqtt_client_config_t mqtt_config ={
        .broker.address.uri = "mqtt://192.168.0.100",
    };
    //Skapar klient för hantering av event
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_config);
    //Initialiserar registering av event
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    //Startar mqtt klient
    esp_mqtt_client_start(client);
}