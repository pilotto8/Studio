/* Multi-Device Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_heap_caps.h>
#include <esp_system.h>

#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_schedule.h>
#include <esp_rmaker_scenes.h>

#include <app_network.h>
#include <app_insights.h>

#include "app_priv.h"

static const char *TAG = "app_main";

esp_rmaker_device_t *light_device;
esp_rmaker_device_t *fan_device;
esp_rmaker_device_t *temp_sensor_device;
power_socket_t power_socket[4] = {
    [0] = {
        .name = "Power socket 0",
        .state = DEFAULT_SWITCH_POWER,
        .output = 12,
        .input = 4,
    },
    [1] = {
        .name = "Power socket 1",
        .state = DEFAULT_SWITCH_POWER,
        .output = 14,
        .input = 16,
    },
    [2] = {
        .name = "Power socket 2",
        .state = DEFAULT_SWITCH_POWER,
        .output = 27,
        .input = 17,
    },
    [3] = {
        .name = "Power socket 3",
        .state = DEFAULT_SWITCH_POWER,
        .output = 26,
        .input = 5,
    },
};

/* Callback to handle commands received from the RainMaker cloud */
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
                          const esp_rmaker_param_val_t val, void *priv_data, esp_rmaker_write_ctx_t *ctx)
{
    if (ctx)
    {
        ESP_LOGI(TAG, "Received write request via : %s", esp_rmaker_device_cb_src_to_str(ctx->src));
    }
    const char *device_name = esp_rmaker_device_get_name(device);
    const char *param_name = esp_rmaker_param_get_name(param);
    if (strcmp(param_name, ESP_RMAKER_DEF_POWER_NAME) == 0)
    {
        ESP_LOGI(TAG, "Received value = %s for %s - %s",
                 val.val.b ? "true" : "false", device_name, param_name);

        for (uint8_t soc = 0; soc < 4; soc++)
        {
            if (strcmp(device_name, power_socket[soc].name) == 0)
            {
                app_driver_set_state(&power_socket[soc], val.val.b);
                break;
            }
        }
    }
    else if (strcmp(param_name, ESP_RMAKER_DEF_BRIGHTNESS_NAME) == 0)
    {
        ESP_LOGI(TAG, "Received value = %d for %s - %s",
                 val.val.i, device_name, param_name);
    }
    else if (strcmp(param_name, ESP_RMAKER_DEF_SPEED_NAME) == 0)
    {
        ESP_LOGI(TAG, "Received value = %d for %s - %s",
                 val.val.i, device_name, param_name);
    }
    else
    {
        /* Silently ignoring invalid params */
        return ESP_OK;
    }
    esp_rmaker_param_update(param, val);
    return ESP_OK;
}

void app_main()
{
    /* Initialize Application specific hardware drivers and
     * set initial state.
     */
    ESP_LOGI(TAG, "main_task: free heap before driver init: %u", esp_get_free_heap_size());
    app_driver_init();
    ESP_LOGI(TAG, "main_task: free heap after driver init: %u", esp_get_free_heap_size());
    ESP_LOGI(TAG, "main_task: stack high water after driver init: %u", (unsigned)uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(pdMS_TO_TICKS(50));
    for (uint8_t soc = 0; soc < 4; soc++)
    {
        app_driver_set_state(&power_socket[soc], DEFAULT_SWITCH_POWER);
    }
    ESP_LOGI(TAG, "main_task: free heap after setting initial states: %u", esp_get_free_heap_size());
    vTaskDelay(pdMS_TO_TICKS(20));

    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    ESP_LOGI(TAG, "main_task: free heap after nvs init: %u", esp_get_free_heap_size());
    ESP_LOGI(TAG, "main_task: stack high water after nvs init: %u", (unsigned)uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(pdMS_TO_TICKS(50));

    // ESP_LOGI(TAG, "flag1");

    /* Initialize Wi-Fi. Note that, this should be called before esp_rmaker_node_init()
     */
    ESP_LOGI(TAG, "main_task: free heap before network init: %u", esp_get_free_heap_size());
    app_network_init();
    ESP_LOGI(TAG, "main_task: free heap after network init: %u", esp_get_free_heap_size());
    ESP_LOGI(TAG, "main_task: stack high water after network init: %u", (unsigned)uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(pdMS_TO_TICKS(50));

    /* Initialize the ESP RainMaker Agent.
     * Note that this should be called after app_network_init() but before app_network_start()
     * */
    esp_rmaker_config_t rainmaker_cfg = {
        .enable_time_sync = false,
    };
    esp_rmaker_node_t *node = esp_rmaker_node_init(&rainmaker_cfg, "ESP RainMaker Multi Device", "Multi Device");
    if (!node)
    {
        ESP_LOGE(TAG, "Could not initialise node. Aborting!!!");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        abort();
    }
    ESP_LOGI(TAG, "main_task: free heap after rmaker node init: %u", esp_get_free_heap_size());
    ESP_LOGI(TAG, "main_task: stack high water after rmaker node init: %u", (unsigned)uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(pdMS_TO_TICKS(50));

    /* Create a Switch device and add the relevant parameters to it */
    for (uint8_t soc = 0; soc < 4; soc++)
    {
        power_socket[soc].device = esp_rmaker_switch_device_create(power_socket[soc].name, NULL, DEFAULT_SWITCH_POWER);
        if (power_socket[soc].device) {
            esp_rmaker_device_add_cb(power_socket[soc].device, write_cb, NULL);
            esp_rmaker_node_add_device(node, power_socket[soc].device);
        } else {
            ESP_LOGW(TAG, "Failed to create device for %s", power_socket[soc].name);
        }
    }

    /* Create a Light device and add the relevant parameters to it */
    light_device = esp_rmaker_lightbulb_device_create("Light", NULL, DEFAULT_LIGHT_POWER);
    esp_rmaker_device_add_cb(light_device, write_cb, NULL);
    esp_rmaker_device_add_param(light_device, esp_rmaker_brightness_param_create(ESP_RMAKER_DEF_BRIGHTNESS_NAME, DEFAULT_LIGHT_BRIGHTNESS));
    esp_rmaker_device_add_attribute(light_device, "Serial Number", "012345");
    esp_rmaker_device_add_attribute(light_device, "MAC", "xx:yy:zz:aa:bb:cc");
    esp_rmaker_node_add_device(node, light_device);

    /* Create a Fan device and add the relevant parameters to it */
    fan_device = esp_rmaker_fan_device_create("Fan", NULL, DEFAULT_FAN_POWER);
    esp_rmaker_device_add_cb(fan_device, write_cb, NULL);
    esp_rmaker_device_add_param(fan_device, esp_rmaker_speed_param_create(ESP_RMAKER_DEF_SPEED_NAME, DEFAULT_FAN_SPEED));
    esp_rmaker_node_add_device(node, fan_device);

    /* Create a Temperature Sensor device and add the relevant parameters to it */
    temp_sensor_device = esp_rmaker_temp_sensor_device_create("Temperature Sensor", NULL, app_get_current_temperature());
    esp_rmaker_node_add_device(node, temp_sensor_device);

    ESP_LOGI(TAG, "Devices added");

    /* Enable OTA */
    esp_rmaker_ota_enable_default();

    /* Enable timezone service which will be require for setting appropriate timezone
     * from the phone apps for scheduling to work correctly.
     * For more information on the various ways of setting timezone, please check
     * https://rainmaker.espressif.com/docs/time-service.html.
     */
    esp_rmaker_timezone_service_enable();

    /* Enable scheduling. */
    esp_rmaker_schedule_enable();

    /* Enable Scenes */
    esp_rmaker_scenes_enable();

    /* Enable Insights. Requires CONFIG_ESP_INSIGHTS_ENABLED=y */
    app_insights_enable();

    /* Start the ESP RainMaker Agent */
    esp_rmaker_start();
    ESP_LOGI(TAG, "main_task: free heap after rmaker start: %u", esp_get_free_heap_size());
    ESP_LOGI(TAG, "main_task: stack high water after rmaker start: %u", (unsigned)uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(pdMS_TO_TICKS(50));

    /* Start the Wi-Fi.
     * If the node is provisioned, it will start connection attempts,
     * else, it will start Wi-Fi provisioning. The function will return
     * after a connection has been successfully established
     */
    ESP_LOGI(TAG, "main_task: free heap before network start: %u", esp_get_free_heap_size());
    err = app_network_start(POP_TYPE_RANDOM);
    ESP_LOGI(TAG, "main_task: free heap after network start attempt: %u", esp_get_free_heap_size());
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Could not start Wifi. Aborting!!!");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        abort();
    }
}
