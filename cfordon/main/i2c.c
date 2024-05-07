#include "i2c.h"
#include "driver/i2c_master.h"


i2c_master_bus_config_t i2c_config_fordon = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = -1,
    .scl_io_num = GPIO_NUM_8,
    .sda_io_num = GPIO_NUM_10,
    .glitch_ignore_cnt = 7,
};

i2c_device_config_t device_config = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = MOTODRIVER_ADDR,
    .scl_speed_hz = STD_SCL_SPEED,
};

i2c_master_dev_handle_t device_handle_fordon;
i2c_master_bus_handle_t bus_handle_fordon;

bool emergency_stop = false;
int current_speed = 0;
int max_speed = 100;

//initiera i2c kontrollern
void init_i2c() {
    //skapa kanalen
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config_fordon, &bus_handle_fordon));
    //bädda för enhet i fråga
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle_fordon, &device_config, &device_handle_fordon));
    startup_register();

}

void write_address(uint8_t value, Register address){
    uint8_t buf[2] = {(uint8_t)address, value};
    ESP_ERROR_CHECK(i2c_master_transmit(device_handle_fordon, buf, sizeof(buf), -1));
}

void read_address(Register address){
    uint8_t buf[1] = {(uint8_t)address};
    uint8_t buffer[2]; 
    ESP_ERROR_CHECK(i2c_master_transmit_receive(device_handle_fordon, buf, sizeof(buf), buffer, 2, -1));
    printf("Läste: %u\n", buffer[0]);

}

void startup_register() {
    uint8_t mode1 = 0x01;
    uint8_t mode2 = 0x14;
    uint8_t ledout = 0xFF;

    write_address(mode1, MODE1);
    write_address(mode2, MODE2);
    vTaskDelay(6/portTICK_PERIOD_MS);
    write_address(ledout, LEDOUT0);
    write_address(ledout, LEDOUT1);

    printf("\n Läser startregister höhöh \n");

    read_address(MODE1);
    read_address(MODE2);
    read_address(LEDOUT0);
    read_address(LEDOUT1);

    printf("\n Klart \n");
}

//Körfunktioner
void forward(uint8_t speed){
    clear_backwards();
    printf("SPEED: %u", speed);
    write_address(speed, PWM0);
    write_address(speed, PWM2);
    write_address(speed, PWM4);
    write_address(speed, PWM6);
}

void backward(uint8_t speed){
    clear_forward();
    write_address(speed, PWM1);
    write_address(speed, PWM3);
    write_address(speed, PWM5);
    write_address(speed, PWM7);
}

void clear_forward(){
    write_address(0, PWM0);
    write_address(0, PWM2);
    write_address(0, PWM4);
    write_address(0, PWM6);
}
void clear_backwards(){
    write_address(0, PWM1);
    write_address(0, PWM3);
    write_address(0, PWM5);
    write_address(0, PWM7);
}
int calculate_speed(int speed){    
    int converted = (speed*65)/100;
    return converted + 190;
}

//MQTT
void set_speed(int speed){
    if(!emergency_stop){
        if(speed == 0){stop_vechicle();} else{
            if(speed <= 100 && speed >= -100){
            if(speed > 0){
                if(speed <= max_speed){
                    current_speed = speed;
                    forward((uint8_t)calculate_speed(speed));
                }else {
                    set_speed(max_speed);
                }
            }else{
                if(speed >= (max_speed * -1)){
                    current_speed = speed;
                    backward((uint8_t)calculate_speed(speed * -1));
                } else{
                    set_speed(max_speed);
                }
            }
            }
        }
    }
}


void stop_vechicle(){
    write_address(0, PWM0);
    write_address(0, PWM2);
    write_address(0, PWM4);
    write_address(0, PWM6);
    write_address(0, PWM1);
    write_address(0, PWM3);
    write_address(0, PWM5);
    write_address(0, PWM7);
}
void set_emergency_stop(bool tf){
    if(tf == true || tf == false){
        emergency_stop = tf;
        if(emergency_stop){
            stop_vechicle();
        }
    }
}
short get_emergency_stop(){
    return emergency_stop;
}
void set_max_speed(int speed){
    max_speed = speed;
    if(current_speed >= 0){
        if(max_speed < current_speed){
            set_speed(max_speed);
        }
    }else{
        if((max_speed * -1) > current_speed){
            set_speed((max_speed * -1));
        }
    }
}
int get_max_speed(){
    return max_speed;
}
