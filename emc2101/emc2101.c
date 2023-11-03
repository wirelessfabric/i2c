// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit EMC2101 Fan Controller and Temperature Sensor
// Data Sheet:
// https://ww1.microchip.com/downloads/en/DeviceDoc/2101.pdf
//
// $ gcc emc2101.c ../io/i2c_dev_io.c -I../io -o emc2101
// $ ./emc2101
//
// $ gcc -DCONFIG_I2C_HID_IO
//       -I../io -I../mcp2221a
//       emc2101.c
//       ../mcp2221a/mcp2221a_hid.c
//       ../mcp2221a/mcp2221a_i2c.c
//       ../io/i2c_hid_io.c
//       -lhidapi-libusb -L/usr/lib/arm-linux-gnueabihf
//       -o emc2101_i2c_hid
// $ ./emc2101_i2c_hid
//

#include <stdio.h>
#include <stdlib.h>
#include "emc2101.h"

static inline float ctof(float celsius) {
    return (celsius * 1.8f) + 32.f;
}

static float emc2101_read_internal_temperature(void* context) {
    return i2c_read_s8(context, EMC2101_REG_INT_TEMP);
}

static float emc2101_read_external_temperature(void* context) {
    int8_t msb = i2c_read_s8(context, EMC2101_REG_EXT_TEMP_MSB);
    uint8_t lsb = i2c_read_u8(context, EMC2101_REG_EXT_TEMP_LSB);

    // check for diode fault conditions
    if (msb == EMC2101_EXT_TEMP_MSB_DIODE_FAULT_CHECK) {
        if (lsb == EMC2101_EXT_TEMP_LSB_DIODE_FAULT_OPEN)
            return EMC2101_DIODE_FAULT_OPEN;
        if (lsb == EMC2101_EXT_TEMP_LSB_DIODE_FAULT_SHORT)
            return EMC2101_DIODE_FAULT_SHORT;
    }

    int16_t raw = (int16_t)(((uint16_t)msb << 3) |
                            ((uint16_t)lsb >> 5));
    return (float)(int)raw * 0.125f;
}

static void loop(void* context) {
    while (true) {
        float internal = emc2101_read_internal_temperature(context);
        float external = emc2101_read_external_temperature(context);

        if (external == EMC2101_DIODE_FAULT_OPEN)
            printf("External Temperature: Diode Fault (Open Condition)\n");
        else if (external == EMC2101_DIODE_FAULT_SHORT)
            printf("External Temperature: Diode Fault (Short Condition)\n");
        else
            printf("External Temperature: %6.2f C %6.2f F\n", external, ctof(external));

        printf("Internal Temperature: %6.2f C %6.2f F\n\n", internal, ctof(internal));

        i2c_sleep(2000000);
    }
}

static int setup(void* context) {
    uint8_t product_id = i2c_read_u8(context, EMC2101_REG_ID);
    if (product_id != EMC2101_ID_DEFAULT) {
        printf("ERROR: setup() unexpected product id 0x%02x\n", product_id);
        return EMC2101_INVALID_ID;
    }

    uint8_t desired_rate = EMC2101_RATE_32_HZ;
    if (i2c_write_reg_u8(context, EMC2101_REG_DATA_RATE, desired_rate) < 0) {
        printf("ERROR: setup() failed to write data rate register\n");
        return EMC2101_WRITE_DATA_RATE_ERROR;
    }

    uint8_t data_rate = i2c_read_u8(context, EMC2101_REG_DATA_RATE);
    if (data_rate != desired_rate) {
        printf("ERROR: setup() invalid data rate 0x%02x\n", data_rate);
        return EMC2101_INVALID_DATA_RATE;
    }

    return EMC2101_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, EMC2101_I2C_ADDR);

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
