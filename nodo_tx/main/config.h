#pragma once

/* ---------- Pines de sensores (Heltec V3) ---------- */
#define PIN_ONE_WIRE   4
#define PIN_TDS        2
#define PIN_PH         5
#define PIN_OD         6
#define PIN_TSS        7
/* RTC DS3231 */
#define RTC_PIN_SDA    19
#define RTC_PIN_SCL    20


/* ---------- pH ---------- */
#define PH_MODE            1
#define PH_LIN_M          (-5.70f)
#define PH_LIN_B           21.34f
#define PH_DIVIDER_RATIO    0.6f

/* ---------- ADC ---------- */
#define ADC_SAMPLES         16

#define SIM_FULLSCALE_MV    3100.0f

/* ---------- Sensores simulados ---------- */
#define OD_MAX_MGL          20.0f
#define SST_MAX_MGL         1000.0f

/* ---------- Calibracion pH ---------- */
#define PH_CAL_PH_A         7.00f
#define PH_CAL_MV_A         2500.0f

#define PH_CAL_PH_B         4.00f
#define PH_CAL_MV_B         3030.0f

#define PH_CAL_TEMP_C       25.0f

#define PH_CAL_MODE         0

#define PH_ERROR_VALUE      (-1.0f)

#define TX_PERIOD_MS        5000
#define TX_TIMEOUT_MS       3000

#define TEMP_ERROR_VALUE    (-127.0f)