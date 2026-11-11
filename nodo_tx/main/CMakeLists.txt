idf_component_register(
    SRCS
        "main.c"
        "ds18b20.c"
        "sensors.c"
        "ph_sensor.c"
        "telemetry.c"
        "rtc_ds3231.c"
    INCLUDE_DIRS "."
    REQUIRES
        driver
        esp_driver_i2c
        esp_adc
        esp_timer
        esp_rom
        biosense_common
        sx1262
)