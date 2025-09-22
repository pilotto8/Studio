/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>

#include <iot_button.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_standard_params.h>
#include <driver/gpio.h>

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"
#include "esp_log.h"
#include "esp_psram.h"

static const char *TAG = "app_driver";

/* This is the button that is used for toggling the power */
// #define BUTTON_GPIO CONFIG_EXAMPLE_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL 0
/* This is the GPIO on which the power will be set */
// #define OUTPUT_GPIO CONFIG_EXAMPLE_OUTPUT_GPIO

/* These values correspoind to H,S,V = 120,100,10 */
#define DEFAULT_RED 0
#define DEFAULT_GREEN 25
#define DEFAULT_BLUE 0

#define WIFI_RESET_BUTTON_TIMEOUT 3
#define FACTORY_RESET_BUTTON_TIMEOUT 10

// static bool g_power_state = DEFAULT_SWITCH_POWER;
static float g_temperature = DEFAULT_TEMPERATURE;
static TimerHandle_t sensor_timer;

static void app_sensor_update(TimerHandle_t handle)
{
    static float delta = 0.5;
    g_temperature += delta;
    if (g_temperature > 99)
    {
        delta = -0.5;
    }
    else if (g_temperature < 1)
    {
        delta = 0.5;
    }
    esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(temp_sensor_device, ESP_RMAKER_PARAM_TEMPERATURE),
        esp_rmaker_float(g_temperature));
}

float app_get_current_temperature()
{
    return g_temperature;
}

esp_err_t app_sensor_init(void)
{
    g_temperature = DEFAULT_TEMPERATURE;
    sensor_timer = xTimerCreate("app_sensor_update_tm", (REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                                pdTRUE, NULL, app_sensor_update);
    if (sensor_timer)
    {
        xTimerStart(sensor_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}

static void app_indicator_set(bool state)
{
    (void)state; // placeholder: LED indicator optional
}

static void app_indicator_init(void)
{
    // Optional: initialize RGB indicator if present
}

static void push_btn_cb(void *arg)
{
    power_socket_t *power_soket = (power_socket_t *)arg;
    bool new_state = !power_soket->state;
    app_driver_set_state(power_soket, new_state);
    esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(power_soket->device, ESP_RMAKER_PARAM_POWER),
        esp_rmaker_bool(new_state));
}

static void set_power_state(power_socket_t *power_soket, bool target)
{
    gpio_set_level(power_soket->output, target);
    app_indicator_set(target);
}

void app_driver_init()
{
    ESP_LOGI(TAG, "app_driver_init: start");
    /* Configure power outputs first */
    uint64_t mask = 0;
    for (uint8_t soc = 0; soc < 4; soc++) {
        mask |= ((uint64_t)1 << power_socket[soc].output);
    }
    ESP_LOGI(TAG, "app_driver_init: configuring outputs individually, pin_bit_mask=0x%llx", mask);
#if (CONFIG_SPIRAM || CONFIG_SPIRAM_SUPPORT)
    bool psram_ready = esp_psram_is_initialized();
    if (psram_ready) {
        ESP_LOGI(TAG, "app_driver_init: PSRAM detected and initialized; will avoid reconfiguring PSRAM pins if used");
    }
#else
    bool psram_ready = false;
#endif
    for (uint8_t soc = 0; soc < 4; soc++) {
        int out = power_socket[soc].output;
        /* Avoid reconfiguring PSRAM pins after PSRAM init on boards that use them. */
        if (psram_ready && (out == 35 || out == 36 || out == 37 || out == 38)) {
            ESP_LOGW(TAG, "skipping PSRAM pin %d (soc=%d)", out, soc);
            continue;
        }
        ESP_LOGD(TAG, "reset/config output pin %d (soc=%d)", out, soc);
        gpio_reset_pin(out);
        gpio_set_direction(out, GPIO_MODE_OUTPUT);
        gpio_set_level(out, 0);
    }
    ESP_LOGI(TAG, "app_driver_init: outputs configured");

    /* Install ISR service (idempotent) and create buttons with lower log noise. */
    esp_err_t isr_ret = gpio_install_isr_service(0);
    if (isr_ret == ESP_OK) {
        ESP_LOGI(TAG, "gpio ISR service installed");
    } else if (isr_ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGD(TAG, "gpio ISR service already present");
    } else {
        ESP_LOGW(TAG, "gpio_install_isr_service failed: 0x%04x", isr_ret);
    }

    ESP_LOGI(TAG, "creating button handlers");
    esp_log_level_t prev_level = esp_log_level_get("gpio");
    esp_log_level_set("gpio", ESP_LOG_NONE);
    for (uint8_t soc = 0; soc < 4; soc++) {
        int in = power_socket[soc].input;
        ESP_LOGD(TAG, "creating button for input GPIO %d (soc=%d)", in, soc);
        button_handle_t btn_handle = iot_button_create(in, BUTTON_ACTIVE_LEVEL);
        if (btn_handle) {
            iot_button_set_evt_cb(btn_handle, BUTTON_CB_TAP, push_btn_cb, &power_socket[soc]);
            ESP_LOGD(TAG, "button created soc=%d", soc);
        } else {
            ESP_LOGW(TAG, "failed to create button for soc=%d", soc);
        }
    }
    esp_log_level_set("gpio", prev_level);
     /* Yield briefly to allow system housekeeping and avoid WDT while creating buttons */
     vTaskDelay(pdMS_TO_TICKS(50));
    ESP_LOGW(TAG, "bitmask: 0x%llx", mask);
    app_indicator_init();
    app_sensor_init();
}

int IRAM_ATTR app_driver_set_state(power_socket_t *power_soket, bool state)
{
    if (power_soket->state != state)
    {
        power_soket->state = state;
        set_power_state(power_soket, state);
    }
    return ESP_OK;
}

bool app_driver_get_state(power_socket_t *power_soket)
{
    return power_soket->state;
}
