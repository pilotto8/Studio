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
// #define DEFAULT_LIGHT_BRIGHTNESS 25
#define DEFAULT_HUE 180
#define DEFAULT_SATURATION 100
#define DEFAULT_BRIGHTNESS 25

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
   uint16_t hue;
   uint16_t saturation;
   uint16_t brightness;
} rgb_light_t;

typedef struct
{
   const char *name;
   bool state;
   esp_rmaker_device_t *device;
   const uint8_t output;
   const uint8_t input;
   const enum rm_device_type {
      D_POWER_SOCKET,
      D_RGB,
   } type;

   uint16_t hue;
   uint16_t saturation;
   uint16_t brightness;
} rm_device_t;

extern rm_device_t power_socket[4];
extern rm_device_t rgb_light;

void app_driver_init();

// POWER SOCKETS
int app_power_socket_set_power(rm_device_t *power_soket, bool state);
bool app_power_socket_get_state(rm_device_t *power_soket);

// RGB LIGHT
esp_err_t app_light_init(rm_device_t *rgb_dev);
esp_err_t app_light_set(rm_device_t *rgb_dev, uint32_t hue, uint32_t saturation, uint32_t brightness);
esp_err_t app_light_set_power(rm_device_t *rgb_dev, bool power);
esp_err_t app_light_set_brightness(rm_device_t *rgb_dev, uint16_t brightness);
esp_err_t app_light_set_hue(rm_device_t *rgb_dev, uint16_t hue);
esp_err_t app_light_set_saturation(rm_device_t *rgb_dev, uint16_t saturation);

// TEMPERATURE
float app_get_current_temperature();
