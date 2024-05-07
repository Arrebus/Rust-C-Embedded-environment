#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"
#include <string.h> 
#include "freertos/FreeRTOS.h" 
#include "esp_system.h"
#include "esp_wifi.h" 
#include "esp_log.h" 
#include "esp_event.h"
#include "lwip/err.h"
#include "lwip/sys.h"

void hw_conf();
void wifi_conf();
esp_err_t wifi_connect();

#endif // WIFI_H
