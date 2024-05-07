#ifndef I2C_H
#define I2C_H

#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include <stdio.h>

#define DATA_LENGTH 100
#define MOTODRIVER_ADDR 0x15
#define STD_SCL_SPEED 100000
#define VEHICLE_ID 3

extern bool emergency_stop;
extern int max_speed;
extern int current_speed;

extern i2c_master_bus_config_t i2c_config_fordon;
extern i2c_device_config_t device_config;
extern i2c_master_dev_handle_t device_handle_fordon;
extern i2c_master_bus_handle_t bus_handle_fordon;

typedef enum Register {
    MODE1 = 0x00,
    MODE2 = 0x01,
    PWM0 = 0x02,
    PWM1 = 0x03,
    PWM2 = 0x04,
    PWM3 = 0x05,
    PWM4 = 0x06,
    PWM5 = 0x07,
    PWM6 = 0x08,
    PWM7 = 0x09,
    GRPPWM = 0x0A,
    GRPFREQ = 0x0B,
    LEDOUT0 = 0x0C,
    LEDOUT1 = 0x0D,
    SUBADR1 = 0x0E,
    SUBADR2 = 0x0F,
    SUBADR3 = 0x10,
    ALLCALLADR = 0x11,
} Register;

void init_i2c();
void startup_register();
void write_address(uint8_t value, Register address);
void read_address(Register address);

void clear_forward();
void clear_backwards();
int calculate_speed();

//MQTT-funktioner
void set_speed(int speed);
void forward(uint8_t speed);
void backward(uint8_t speed);
void stop_vechicle();
void set_emergency_stop(bool tf);
short get_emergency_stop();
void set_max_speed(int speed);
int get_max_speed();




#endif // I2C_H
