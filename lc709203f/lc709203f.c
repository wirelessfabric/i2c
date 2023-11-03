// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit LC709203F LiPoly / LiIon Fuel Gauge and Battery Monitor
// Smart LiB Gauge Battery Fuel Gauge LSI For 1-Cell Lithium-ion/Polymer (Li+)
// Data Sheet:
// See: https://cdn-learn.adafruit.com/assets/assets/000/094/597/original/LC709203F-D.PDF?1599248750
//
// $ gcc lc709203f.c ../io/i2c_dev_io.c -I../io -o lc709203f
// $ ./lc709203f

#include <stdio.h>
#include <stdlib.h>
#include "lc709203f.h"

static inline float ktoc(float kelvin) {
    return kelvin - 273.15f;
}

static inline float ctof(float celsius) {
    return (celsius * 1.8f) + 32.f;
}

static uint8_t lc709203f_crc8(uint8_t *data, size_t size) {
    const uint8_t polynomial = 0x07;
    uint8_t crc = 0x00;

    for (size_t i=0; i < size; i++) {
        crc ^= data[i];
        for (int j=0; j < 8; j++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ polynomial;
            else
                crc = (crc << 1);
        }
    }

    return crc;
}

static int lc709203f_set(void* context, int reg, uint16_t value) {
    uint8_t command[32] = {0};
    command[0] = i2c_addr(context) << 1;
    command[1] = reg;
    command[2] = value & 0xFF;
    command[3] = value >> 8;
    command[4] = lc709203f_crc8(command, 4);

    if (i2c_dump(context))
        i2c_dump_buffer(context, "> ", command);

    return i2c_write(context, &command[1], 4);
}

static int lc709203f_set_power_mode(void* context, uint16_t mode) {
    return lc709203f_set(context, LC709203F_REG_POWER_MODE, mode);
}

static int lc709203f_set_apa(void* context, uint16_t apa) {
    return lc709203f_set(context, LC709203F_REG_APA, apa);
}

static int lc709203f_set_battery_profile(void* context, uint16_t profile) {
    return lc709203f_set(context, LC709203F_REG_BATTERY_PROFILE, profile);
}

static int lc709203f_set_temperature_mode(void* context, uint16_t mode) {
    return lc709203f_set(context, LC709203F_REG_STATUS_BIT, mode);
}

static int lc709203f_set_thermistor_b(void* context, uint16_t mode) {
    return lc709203f_set(context, LC709203F_REG_THERMISTOR_B, mode);
}

static int lc709203f_get(void* context, int reg, uint16_t* value) {
    int result = smbus_read_reg_u16_be(context, reg, value);
    if (result < 0 && i2c_debug(context))
        printf("lc709203f_get: result %d\n", result);
    return result;
}

static int lc709203f_get_ic_version(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_IC_VERSION, value);
}

static int lc709203f_get_power_mode(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_POWER_MODE, value);
}

static int lc709203f_get_current_flow_mode(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_CURRENT_FLOW_MODE, value);
}

static int lc709203f_get_alarm_percentage(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_ALARM_PERCENT, value);
}

static int lc709203f_get_alarm_voltage(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_ALARM_VOLTAGE, value);
}

static int lc709203f_get_cell_indicator_to_empty(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_CELL_ITE, value);
}

static int lc709203f_get_cell_voltage(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_CELL_VOLTAGE, value);
}

static int lc709203f_get_thermistor_b(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_THERMISTOR_B, value);
}

static int lc709203f_get_temperature_sensor(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_STATUS_BIT, value);
}

static int lc709203f_get_cell_temperature(void* context, uint16_t* value) {
    return lc709203f_get(context, LC709203F_REG_CELL_TEMPERATURE, value);
}

static void lc709203f_show_cell_temperature(void* context) {
    uint16_t dK, mode, thermistor_b;
    int result;

    result = lc709203f_get_cell_temperature(context, &dK);
    if (result == I2C_SUCCESS) {
        float K = (float)dK / 10.f;
        float C = ktoc(K);
        float F = ctof(C);

        printf("Cell Temperature: ");
        if (i2c_debug(context))
            printf("0x%04x (%d) dK %5.2f K ", dK, dK, K);
        printf("%5.2f C %5.2f F\n", C, F);
    }

    result = lc709203f_get_temperature_sensor(context, &mode);
    if (result == I2C_SUCCESS) {
        printf("Temperature Sensor: ");
        if (mode == LC709203F_THERMISTOR_MODE)
            printf("Thermistor\n");
        else
            printf("I2C\n");
    }

    result = lc709203f_get_thermistor_b(context, &thermistor_b);
    if (result == I2C_SUCCESS)
        printf("Thermistor B: 0x%x (%d)\n", thermistor_b, thermistor_b);
}

static void lc709203f_show_cell_indicator_to_empty(void* context) {
    uint16_t value;
    int result = lc709203f_get_cell_indicator_to_empty(context, &value);
    if (result == I2C_SUCCESS) {
        float percentage = (float)value / 10.f;
        printf("Cell Indicator To Empty: %4.2f %%\n", percentage);
    }
}

static void lc709203f_show_cell_voltage(void* context) {
    uint16_t value;
    int result = lc709203f_get_cell_voltage(context, &value);
    if (result == I2C_SUCCESS) {
        float mV = (float)value;
        printf("Cell Voltage: %4.2f V\n", mV / 1000.f);
    }
}

static void lc709203f_show_alarm_voltage(void* context) {
    uint16_t value;
    int result = lc709203f_get_alarm_voltage(context, &value);
    if (result == I2C_SUCCESS) {
        printf("Alarm Voltage: ");
        if (value) {
            float mV = (float)value;
            printf("%4.2f V\n", mV / 1000.f);
        } else {
            printf("Disabled\n");
        }
    }
}

static void lc709203f_show_alarm_percentage(void* context) {
    uint16_t value;
    int result = lc709203f_get_alarm_percentage(context, &value);
    if (result == I2C_SUCCESS) {
        float percentage = (float)value;
        printf("Alarm Percentage: %4.2f %%\n", percentage);
    }
}

static void lc709203f_show_current_flow_mode(void* context) {
    uint16_t mode;
    int result = lc709203f_get_current_flow_mode(context, &mode);
    if (result == I2C_SUCCESS) {
        printf("Current Direction: ");
        if (mode == LC709203F_FLOW_AUTO)
            printf("Auto");
        else if (mode == LC709203F_FLOW_CHARGE)
            printf("Charge");
        else if (mode == LC709203F_FLOW_DISCHARGE)
            printf("Discharge");
        else
            printf("Invalid 0x%04x", mode);
        printf(" Mode\n");
    }
}

static void lc709203f_show_power_mode(void* context) {
    uint16_t mode;
    int result = lc709203f_get_power_mode(context, &mode);
    if (result == I2C_SUCCESS) {
        printf("Power Mode: ");
        if (mode == LC709203F_POWER_OPERATIONAL)
            printf("Operational\n");
        else if (mode == LC709203F_POWER_SLEEP)
            printf("Sleep\n");
        else
            printf("Invalid 0x%04x\n", mode);
    }
}

static void loop(void* context) {
    while (true) {
        lc709203f_show_power_mode(context);
        lc709203f_show_current_flow_mode(context);
        lc709203f_show_alarm_percentage(context);
        lc709203f_show_alarm_voltage(context);
        lc709203f_show_cell_voltage(context);
        lc709203f_show_cell_indicator_to_empty(context);
        lc709203f_show_cell_temperature(context);
        i2c_sleep(2000000);
        printf("\n");
    }
}

static int setup(void* context) {
    uint16_t ic_version;
    int result;

    result = lc709203f_get_ic_version(context, &ic_version);
    if (result < 0)
        return result;
    else if (ic_version != LC709203F_IC_VERSION) {
        printf("ERROR: setup() unexpected chip id 0x%02x\n", ic_version);
        return LC709203F_INVALID_IC_VERSION;
    }

    result = lc709203f_set_power_mode(context, LC709203F_POWER_OPERATIONAL);
    if (result < 0) {
        printf("ERROR: setup() failed to set operational power mode\n");
        return LC709203F_SET_POWER_MODE_ERROR;
    }

    result = lc709203f_set_apa(context, LC709203F_APA_500MAH);
    if (result < 0) {
        printf("ERROR: setup() failed to set approximate pack size\n");
        return LC709203F_SET_APA_ERROR;
    }

    result = lc709203f_set_battery_profile(context, LC709203F_BATTERY_4700mV);
    if (result < 0) {
        printf("ERROR: setup() failed to set 4.7V battery profile\n");
        return LC709203F_SET_BATTERY_PROFILE_ERROR;
    }

    result = lc709203f_set_temperature_mode(context, LC709203F_THERMISTOR_MODE);
    if (result < 0) {
        printf("ERROR: setup() failed to set I2C sensing mode\n");
        return LC709203F_SET_THERMISTOR_MODE_ERROR;
    }

    result = lc709203f_set_thermistor_b(context, LC709203F_THERMISTOR_B);
    if (result < 0) {
        printf("ERROR: setup() failed to set thermistor B constant\n");
        return LC709203F_SET_THERMISTOR_B_ERROR;
    }

    return LC709203F_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, LC709203F_I2C_ADDR);

    result = i2c_open(&g_context, argc, argv);
    if (result < 0)
        return result;

    result = setup(&g_context);
    if (result < 0)
        printf("ERROR: setup() failed %d\n", result);
    else
        loop(&g_context);

    i2c_close(&g_context);
    return result;
}
