#include "wifi.h"
#include "esp_wifi.h"
#include "esp_log.h"
const char *SSID = "exjobb";
const char *PASSWORD = "password";
int retry_num = 0;
static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data){

    switch(event_id){
        case WIFI_EVENT_STA_START:
            printf("WIFI CONNECTING....\n");
            break;
        case WIFI_EVENT_STA_CONNECTED:
            printf("WiFi CONNECTED\n");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            printf("WiFi lost connection\n");
            if(retry_num<5){
                printf("Retrying to Connect...\n");
                esp_wifi_connect();
                retry_num++;
            }
            break;
        case IP_EVENT_STA_GOT_IP:
            printf("Wifi got IP...\n");
            break;
        default:
            printf("Unknown event!\n");
            break;
    }
    return;
}

esp_err_t wifi_connect() {
    hw_conf();
    wifi_conf();
    
    printf( "Ansluter till wifi med SSID:%s",SSID);
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    
    esp_wifi_connect();
    printf( "Ansluten till Wifi: %s",SSID);
    return ESP_OK;
}

//Konfigurerar/förbereder ESP-IDF för wifi och händelser.
void hw_conf() {
    esp_netif_init(); // starta netif
    esp_event_loop_create_default(); //För händelser
    esp_netif_create_default_wifi_sta(); //skapa wifi sstation
}

//Skapar och tillämpar en wifikonfiguration!
void wifi_conf() {
    //Hämta standardkonfiguration och applicera den på vårt wifi
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&config);

    //För olika events säger vi att vi skall anropa wifi_event_handler
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    //skapa konfiguration och smäll in den i wifistationen!
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = "",
            .password = "",
            
           }
    
        };
    strcpy((char*)wifi_configuration.sta.ssid, SSID);
    strcpy((char*)wifi_configuration.sta.password, PASSWORD);    
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
}
