// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit VCNL4020 Proximity and Light Sensor
// Smart LiB Gauge Battery Fuel Gauge LSI For 1-Cell Lithium-ion/Polymer (Li+)
// Data Sheet:
// See: Adafruit VCNL4020 Proximity and Light Sensor
//
// $ gcc vcnl4020.c ../io/i2c_dev_io.c -I../io -o vcnl4020
// $ ./vcnl4020

#include <stdio.h>
#include <stdlib.h>
#include "vcnl4020.h"

static void loop(void* context) {
    while (true) {
        uint16_t proximity = i2c_read_u16_be(context, VCNL4020_REG_PROXIMITY_RESULT_HIGH);
        uint16_t ambient = i2c_read_u16_be(context, VCNL4020_REG_AMBIENT_RESULT_HIGH);

        printf("Proximity: %d mm\n", proximity);
        printf("Ambient Light: %d lux\n", ambient);

        i2c_sleep(2000000);
        printf("\n");
    }
}

static int setup(void* context) {
    uint8_t product_id;
    int result = i2c_read_reg_u8(context, VCNL4020_REG_PRODUCT_ID, &product_id);
    if (result < 0)
        return result;
    else if (product_id != VCNL4020_PRODUCT_ID) {
        printf("ERROR: setup() unexpected product id 0x%02x\n", product_id);
        return VCNL4020_INVALID_PRODUCT_ID;
    }

    uint8_t proximity_rate = VCNL4020_PROXMITIY_RATE_250_Hz;
    if (i2c_write_reg_u8(context, VCNL4020_REG_PROXIMITY_RATE, proximity_rate) < 0) {
        printf("ERROR: setup() failed to write proximity rate register\n");
        return VCNL4020_SET_PROXIMITY_RATE_ERROR;
    }

    uint8_t led_current = VCNL4020_IR_LED_CURRENT_200_mA;
    if (i2c_write_reg_u8(context, VCNL4020_REG_IR_LED_CURRENT, led_current) < 0) {
        printf("ERROR: setup() failed to write IR LED current register\n");
        return VCNL4020_SET_IR_LED_CURRENT_ERROR;
    }

    uint8_t ambient_parameters = VCNL4020_AMBIENT_PARAMETERS;
    if (i2c_write_reg_u8(context, VCNL4020_REG_AMBIENT_PARAMETERS, ambient_parameters) < 0) {
        printf("ERROR: setup() failed to write ambient parameters register\n");
        return VCNL4020_SET_AMBIENT_PARAMETERS_ERROR;
    }

    return VCNL4020_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, VCNL4020_I2C_ADDR);

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
