#pragma once

#include <stddef.h>
#include "esp_err.h"

esp_err_t rtc_ds3231_init(void);

void rtc_ds3231_get_timestamp(
    char *buffer,
    size_t len
);