#include "rtc_ds3231.h"
#include "config.h"

#include <stdio.h>

#include "driver/i2c_master.h"
#include "esp_log.h"

#define DS3231_ADDR 0x68

static const char *TAG = "RTC";

static i2c_master_bus_handle_t s_bus = NULL;
static i2c_master_dev_handle_t s_dev = NULL;

static int bcd_to_dec(uint8_t b)
{
    return ((b >> 4) * 10) + (b & 0x0F);
}

esp_err_t rtc_ds3231_init(void)
{
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = RTC_PIN_SDA,
        .scl_io_num = RTC_PIN_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(
        i2c_new_master_bus(
            &bus_cfg,
            &s_bus
        )
    );

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DS3231_ADDR,
        .scl_speed_hz = 100000,
    };

    ESP_ERROR_CHECK(
        i2c_master_bus_add_device(
            s_bus,
            &dev_cfg,
            &s_dev
        )
    );

    ESP_LOGI(
        TAG,
        "DS3231 inicializado"
    );

    return ESP_OK;
}

void rtc_ds3231_get_timestamp(
    char *buffer,
    size_t len
)
{
    uint8_t reg = 0x00;
    uint8_t data[7];

    esp_err_t err;

    err = i2c_master_transmit(
        s_dev,
        &reg,
        1,
        1000
    );

    if (err != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "Error seleccionando registro: %s",
            esp_err_to_name(err)
        );

        snprintf(buffer, len, "RTC_ERROR");

        return;
    }

    err = i2c_master_receive(
        s_dev,
        data,
        sizeof(data),
        1000
    );

    if (err != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "Error leyendo RTC: %s",
            esp_err_to_name(err)
        );

        snprintf(buffer, len, "RTC_ERROR");

        return;
    }

    int sec   = bcd_to_dec(data[0] & 0x7F);
    int min   = bcd_to_dec(data[1] & 0x7F);
    int hour  = bcd_to_dec(data[2] & 0x3F);
    int day   = bcd_to_dec(data[4] & 0x3F);
    int month = bcd_to_dec(data[5] & 0x1F);
    int year  = 2000 + bcd_to_dec(data[6]);

    ESP_LOGI(
        TAG,
        "%02d/%02d/%04d %02d:%02d:%02d",
        day,
        month,
        year,
        hour,
        min,
        sec
    );

    snprintf(
        buffer,
        len,
        "%04d-%02d-%02d %02d:%02d:%02d",
        year,
        month,
        day,
        hour,
        min,
        sec
    );
}