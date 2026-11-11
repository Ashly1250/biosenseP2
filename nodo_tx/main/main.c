#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "biosense_radio.h"
#include "sx1262.h"
#include "config.h"
#include "sensors.h"
#include "telemetry.h"
#include "ph_sensor.h"
#include "rtc_ds3231.h"

static const char *TAG = "TX";

#if PH_CAL_MODE
/* ---------------- MODO CALIBRACION DEL pH (sin LoRa) ---------------- */
static void ph_cal_task(void *arg)
{
    while (1) {
        float adc_mv = sensors_read_ph_adc_mv();
        ESP_LOGI(TAG, "pH ADC: %.0f mV | Po del modulo: %.0f mV | pH estimado: %.2f",
                 adc_mv, ph_module_mv(adc_mv), ph_from_adc_mv(adc_mv, 25.0f));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    ESP_LOGW(TAG, "MODO CALIBRACION pH: anota 'Po del modulo' en cada buffer. LoRa desactivado.");
    ESP_ERROR_CHECK(sensors_init());
    xTaskCreate(ph_cal_task, "ph_cal", 4096, NULL, 5, NULL);
}

#else
/* ---------------- OPERACION NORMAL ---------------- */
static void tx_task(void *arg)
{
    TickType_t last = xTaskGetTickCount();
    char frame[BIOSENSE_LORA_MAX_PAYLOAD];

    while (1) {
        sensor_data_t d;
        sensors_read(&d);
        char timestamp[32];
        
        rtc_ds3231_get_timestamp(
            timestamp,
            sizeof(timestamp)
        );
        
        int n = telemetry_format_csv(timestamp,&d, frame, sizeof(frame));
        if (n > 0) {
            ESP_LOGI(TAG, "Trama: %s", frame);
            esp_err_t e = sx1262_send((const uint8_t *)frame, (size_t)n, TX_TIMEOUT_MS);
            if (e == ESP_OK) ESP_LOGI(TAG, "Datos enviados");
            else             ESP_LOGE(TAG, "Error de envio: %s", esp_err_to_name(e));
        }
        vTaskDelayUntil(&last, pdMS_TO_TICKS(TX_PERIOD_MS));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "BIOSENSE - Nodo transmisor");
    ESP_ERROR_CHECK(sensors_init());
    ESP_ERROR_CHECK(biosense_radio_init());
    ESP_ERROR_CHECK(rtc_ds3231_init());
    xTaskCreate(tx_task, "tx_task", 4096, NULL, 5, NULL);
}
#endif
