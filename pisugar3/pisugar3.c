// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// PiSugar3 
// Data Sheet:
// https://github.com/PiSugar/PiSugar/wiki/PiSugar-3-Series
//
// $ gcc pisugar3.c ../io/i2c_dev_io.c -I../io -o pisugar3
// $ ./pisugar3

#include <stdio.h>
#include <stdlib.h>
#include "pisugar3.h"

static struct {
    uint8_t control1;
    uint8_t control2;
    uint8_t tap;
    uint8_t chip_temperature;
    uint16_t battery_voltage;
    uint8_t battery_percentage;
} pisugar3;

static inline float ctof(float celsius) {
    return (celsius * 1.8f) + 32.f;
}

static void show_on_off(const char* text, uint8_t state) {
    printf("%s: %s\n", text, state ? "On" : "Off");
}

static uint8_t pisugar3_show_control1(void* context) {
    uint8_t value;
    if (smbus_read_reg_u8(context, PISUGAR_CONTROL1, &value) < 0)
        return 0;

    uint8_t change = value ^ pisugar3.control1;
    if (change) {
        pisugar3.control1 = value;
        show_on_off("Power Button", change & CONTROL1_POWER_BUTTON);
        show_on_off("Touch Function", change & CONTROL1_TOUCH_FUNCTION);
        show_on_off("Power On When Connected", change & CONTROL1_POWER_ON_CONNECT);
        show_on_off("Output Switch", change & CONTROL1_OUTPUT_SWITCH);
        show_on_off("Charging Switch", change & CONTROL1_CHARGING_SWITCH);
        show_on_off("Power Supply", change & CONTROL1_POWER_SUPPLY);
    }
    return change;
}

static uint8_t pisugar3_show_control2(void* context) {
    uint8_t value;
    if (smbus_read_reg_u8(context, PISUGAR_CONTROL2, &value) < 0)
        return 0;

    uint8_t change = value ^ pisugar3.control2;
    if (change) {
        pisugar3.control2 = value;
        show_on_off("Soft Shutdown Sign", change & CONTROL2_SOFT_SHUTDOWN_SIGN);
        show_on_off("Soft Shutdown", change & CONTROL2_SOFT_SHUTDOWN);
        show_on_off("Auto Hibernate", change & CONTROL2_AUTO_HIBERNATE);
    }
    return change;
}

static uint8_t pisugar3_show_tap(void* context) {
    uint8_t value;
    if (smbus_read_reg_u8(context, PISUGAR_TAP, &value) < 0)
        return 0;

    uint8_t change = value ^ pisugar3.tap;
    if (change) {
        pisugar3.tap = value;
        printf("Custom Button: %d\n", change & TAP_CUSTOM_BUTTON);
    }
    return change;
}

static uint8_t pisugar3_show_chip_temperature(void* context) {
    uint8_t value;
    if (smbus_read_reg_u8(context, PISUGAR_CHIP_TEMPERATURE, &value) < 0)
        return 0;

    if (pisugar3.chip_temperature == value)
        return 0;

    pisugar3.chip_temperature = value;
    float T = (float)(int)value - 40.f;
    printf("Chip Temperature: %5.2f C %5.2f F\n", T, ctof(T));
    return value;
}

static uint16_t pisugar3_show_battery_voltage(void* context) {
    uint16_t value;
    if (smbus_read_reg_u16_be(context, PISUGAR_BATTERY_VOLTAGE, &value) < 0)
        return 0;

    if (pisugar3.battery_voltage == value)
        return 0;

    pisugar3.battery_voltage = value;
    printf("Battery Voltage: %4.2f V\n", (float)value / 1000.f);
    return value;
}

static uint8_t pisugar3_show_battery_percentage(void* context) {
    uint8_t value;
    if (smbus_read_reg_u8(context, PISUGAR_BATTERY_PERCENTAGE, &value) < 0)
        return 0;

    if (pisugar3.battery_percentage == value)
        return 0;

    pisugar3.battery_percentage = value;
    printf("Battery Percentage %d %%\n", value);
    return value;
}

static void loop(void* context) {
    while (true) {
        if (pisugar3_show_control1(context) +
            pisugar3_show_control2(context) +
            pisugar3_show_tap(context) +
            pisugar3_show_chip_temperature(context) +
            pisugar3_show_battery_voltage(context) +
            pisugar3_show_battery_percentage(context))
            printf("\n");
        sleep(1);
    }
}

static int setup(void* context) {
    memset(&pisugar3, 0, sizeof(pisugar3));

    bool old_debug = i2c_debug(context);
    i2c_set_debug(context, false);

    uint8_t data[2];
    int result = i2c_read_reg(context, PISUGAR_VERSION, data, sizeof(data));
    i2c_set_debug(context, old_debug);
    if (result < 0) {
        printf("PiSugar 3 I2C device not responding\n");
        return PISUGAR_GET_VERSION_ERROR;
    }

    uint8_t version = data[0];
    if (version != PISUGAR3_VERSION) {
        printf("Invalid version 0x%02x for PiSugar 3\n", version);
        return PISUGAR_INVALID_VERSION;
    }

    uint8_t model;
    result = smbus_read_reg_u8(context, PISUGAR_MODEL, &model);
    if (result < 0) {
        return PISUGAR_GET_MODEL_ERROR;
    }

    if (model != PISUGAR3_MODEL) {
        printf("Invalid model 0x%02x for PiSugar 3\n", model);
        return PISUGAR_INVALID_MODEL;
    }

    return PISUGAR_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, PISUGAR_I2C_ADDR);

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
