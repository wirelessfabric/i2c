// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Pimoroni Button SHIM for Raspberry Pi
// 
// See: https://www.adafruit.com/product/3582
// See: https://pinout.xyz/pinout/button_shim
//
// $ gcc buttonshim.c ../io/i2c_dev_io.c -I../io -o buttonshim
// $ ./buttonshim

#include <stdio.h>
#include <stdlib.h>
#include "buttonshim.h"

static int buttonshim_set(void* context, int reg, uint8_t value) {
    int result = i2c_write_reg_u8(context, reg, value);
    if (result < 0 && i2c_debug(context))
        printf("buttonshim_set: result %d\n", result);
    return result;
}

static int buttonshim_set_config(void* context, uint8_t value) {
    return buttonshim_set(context, BUTTONSHIM_REG_CONFIG, value);
}

static int buttonshim_set_polarity(void* context, uint8_t value) {
    return buttonshim_set(context, BUTTONSHIM_REG_POLARITY, value);
}

static int buttonshim_set_output(void* context, uint8_t value) {
    return buttonshim_set(context, BUTTONSHIM_REG_OUTPUT, value);
}

static int buttonshim_get(void* context, int reg, uint8_t* value) {
    int result = i2c_read_reg_u8(context, reg, value);
    if (result < 0 && i2c_debug(context))
        printf("buttonshim_get: result %d\n", result);
    return result;
}

static int buttonshim_get_input(void* context, uint8_t* value) {
    return buttonshim_get(context, BUTTONSHIM_REG_INPUT, value);
}

static void loop(void* context) {
    const char* button_name[5] = { "A", "B", "C", "D" ,"E" };
    uint32_t button_state[5] = { 0, 0, 0, 0, 0 };

    while (true) {
        uint8_t input = 0;
        int result = buttonshim_get_input(context, &input);
        if (result == I2C_SUCCESS) {
            for (int key=0; key < 5; key++) {
                uint8_t state = input & 1;
                if (button_state[key] != state) {
                    button_state[key] = state;
                    if (state == 0)
                        printf("Button %s: Pressed\n", button_name[key]);
                    else
                        printf("Button %s: Released\n", button_name[key]);
                }
                input >>= 1;
            }
        }
        i2c_sleep(5000);
    }
}

static int setup(void* context) {
    int result;

    result = buttonshim_set_config(context, BUTTONSHIM_CONFIG_INIT);
    if (result < 0) {
        printf("ERROR: setup() failed to set configuration\n");
        return BUTTONSHIM_SET_CONFIG_ERROR;
    }

    result = buttonshim_set_polarity(context, BUTTONSHIM_POLARITY_INIT);
    if (result < 0) {
        printf("ERROR: setup() failed to set polarity\n");
        return BUTTONSHIM_SET_POLARITY_ERROR;
    }

    result = buttonshim_set_output(context, BUTTONSHIM_OUTPUT_INIT);
    if (result < 0) {
        printf("ERROR: setup() failed to set output\n");
        return BUTTONSHIM_SET_OUTPUT_ERROR;
    }

    return BUTTONSHIM_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, BUTTONSHIM_I2C_ADDR);

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
