#include "main.h"
#include "wifi.h"
#include "mqtt.h"
#include "i2c.h"
void app_main(void)
{
    nvs_flash_init();

    printf("\n---------------- I2C ----------------\n");
    gpio_set_direction(GPIO_NUM_1, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_1, 0);
    init_i2c();
    
    // printf("Tester startar snart!...\n");
    // vTaskDelay(pdMS_TO_TICKS(5000));
    // //Testa set speed
    // printf("***Körtest***\n");
    // printf("100\n");
    // set_speed(100);
    // vTaskDelay(pdMS_TO_TICKS(2000));

    // printf("50\n");
    // set_speed(50);
    // vTaskDelay(pdMS_TO_TICKS(2000));

    // printf("20\n");
    // set_speed(20);
    // vTaskDelay(pdMS_TO_TICKS(2000));

    // printf("75\n");
    // set_speed(75);
    // vTaskDelay(pdMS_TO_TICKS(2000));

    // printf("Bakåt!\n");
    // printf("-75\n");
    // set_speed(-75);
    // vTaskDelay(pdMS_TO_TICKS(2000));

    // printf("-30\n");
    // set_speed(-30);
    // printf("***Körtest klar!***\n");
    // stop_vechicle();

    // printf("+++Maxhastighet test+++\n");
    //  vTaskDelay(pdMS_TO_TICKS(2000));
    // //max speed test
    // printf("Max speed 20\n");
    // set_max_speed(20);
    // vTaskDelay(pdMS_TO_TICKS(2000));
    // printf("speed 100\n");
    // set_speed(100);
    // vTaskDelay(pdMS_TO_TICKS(2000));
    // printf("Max speed -100\n");
    // set_speed(-100);
    // vTaskDelay(pdMS_TO_TICKS(2000));
    // printf("Max speed 100\n");
    // set_max_speed(100);
    // printf("speed 100\n");
    // set_speed(100);
    // vTaskDelay(pdMS_TO_TICKS(2000));
    // printf("Max speed -100\n");
    // set_speed(-100);
    // printf("+++Maxhastighet klar!+++\n");
    
    // //emergency stop test
    // printf("- - - Nödstopp test - - - \n");
    // printf("!!Nödstopp PÅ!!\n");
    // set_emergency_stop(1);
    // printf("Speed: 100\n");
    // set_speed(100);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // printf("Speed: -100\n");
    // set_speed(-100);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // printf("Speed: 70\n");
    // set_speed(70);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // printf("Speed: 70\n");
    // set_speed(70);
    // vTaskDelay(pdMS_TO_TICKS(1000));

    // printf("!!Nödstopp AV!!\n");
    // set_emergency_stop(0);
    // printf("Speed: 100\n");
    // set_speed(100);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // printf("Speed: -100\n");
    // set_speed(-100);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // printf("Speed: 70\n");
    // set_speed(70);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // printf("Speed: 70\n");
    // set_speed(70);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // printf("- - - Nödstopp klar - - - \n");
    // stop_vechicle();
    // printf("\n-------------I2C SLUT -------------\n");

    //anslut wifi!
    if(wifi_connect() != ESP_OK){
        printf("Wifi kunde ej ansluta! :/\n");
        return;
    }
    

    start_mqtt();
}