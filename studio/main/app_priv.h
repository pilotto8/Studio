/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_SWITCH_POWER false
#define DEFAULT_LIGHT_POWER false
#define DEFAULT_LIGHT_BRIGHTNESS 25
#define DEFAULT_FAN_POWER false
#define DEFAULT_FAN_SPEED 3
#define DEFAULT_TEMPERATURE 25.0
#define REPORTING_PERIOD 60 /* Seconds */

extern esp_rmaker_device_t *light__device;
extern esp_rmaker_device_t *fan_device;
extern esp_rmaker_device_t *temp_sensor_device;

typedef struct
{
   const char *name;
   bool state;
   esp_rmaker_device_t *device;
   const uint8_t output;
   const uint8_t input;
} power_socket_t;

extern power_socket_t power_socket[4];

void app_driver_init();
void app_power_soket_init(power_socket_t* power_soket);
int app_driver_set_state(power_socket_t* power_soket, bool state);
bool app_driver_get_state(power_socket_t* power_soket);
float app_get_current_temperature();
