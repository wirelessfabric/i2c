// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Quad Rotary Encoder Breakout with NeoPixels
// https://learn.adafruit.com/adafruit-i2c-quad-rotary-encoder-breakout
// https://www.adafruit.com/product/5752
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/examples/encoder/PID5752_QuadEncoder_demo/PID5752_QuadEncoder_demo.ino
//
// $ gcc neorotary1x4.c
//       adafruit_seesaw.c
//       seesaw_gpio.c
//       seesaw_adc.c
//       seesaw_neopixel.c
//       ../io/i2c_dev_io.c -I../io
//       -o neorotary1x4
// $ ./neorotary1x4

#include <stdio.h>
#include <stdlib.h>
#include "seesaw_gpio.h"
#include "seesaw_encoder.h"
#include "seesaw_neopixel.h"
#include "i2c_io.h"

static void loop(void* context) {
    uint32_t gpio_pin[4] = {
        NEOROTARY_1X4_BUTTON_A_PIN,
        NEOROTARY_1X4_BUTTON_B_PIN,
        NEOROTARY_1X4_BUTTON_C_PIN,
        NEOROTARY_1X4_BUTTON_D_PIN
    };
    int button[4] = {
        NEOROTARY_1X4_BUTTON_A,
        NEOROTARY_1X4_BUTTON_B,
        NEOROTARY_1X4_BUTTON_C,
        NEOROTARY_1X4_BUTTON_D
    };
    const char* button_name[4] = { "A", "B", "C", "D" };
    uint32_t button_state[4] = { 0, 0, 0, 0 };
    uint8_t color[4] = { 0, 0, 0, 0 };
    int32_t encoder_position[4] = { -1, -1, -1, -1 };
    uint32_t bulk_state = 0;

    while (true) {
        for (int key=0; key < 4; key++) {
            int32_t position = 0;
            if (seesaw_encoder_get_position(context, button[key], &position) == 0) {
                // negate to make clockwise rotation positive
                position = -position; 
                if (encoder_position[key] != position) {
                    encoder_position[key] = position;
                    printf("Position %s: %d\n", button_name[key], position);
                    color[key] = position & 0xff;
                    neo_show_color(context, color[key]);
                }
            }
        }

        uint32_t bulk = 0;
        if (seesaw_gpio_get_bulk(context, NEOROTARY_1X4_BUTTON_PINS, &bulk) == 0) {
            if (bulk_state != bulk) {
                bulk_state = bulk;
                for (int key=0; key < 4; key++) {
                    uint32_t state = bulk_state & gpio_pin[key];
                    if (button_state[key] != state) {
                        button_state[key] = state;
                        if (state == 0) {
                            printf("Button %s: Pressed\n", button_name[key]);
                            neo_set_brightness(255);
                            neo_set_led_green(context, key);
                        } else {
                            printf("Button %s: Released\n", button_name[key]);
                            neo_set_brightness(20);
                            neo_set_led_red(context, key);
                        }
                    }
                }
                neo_show(context);
            }
        }

        i2c_sleep(5000);
    }
}

static int setup(void* context) {
    if (seesaw_tiny8x7_setup(context, NEOROTARY_1X4_FW_VERSION, NEOROTARY_1X4_FW_OPTIONS) < 0)
        return SS_TINY8X7_SETUP_ERROR;
    if (seesaw_gpio_setup(context, NEOROTARY_1X4_BUTTON_PINS, SS_GPIO_INPUT_PULLUP) < 0)
        return SS_GPIO_SETUP_ERROR;
    if (seesaw_encoder_set_position(context, NEOROTARY_1X4_BUTTON_A, 0) < 0)
        return SS_ENCODER_A_SETUP_ERROR;
    if (seesaw_encoder_set_position(context, NEOROTARY_1X4_BUTTON_B, 0) < 0)
        return SS_ENCODER_B_SETUP_ERROR;
    if (seesaw_encoder_set_position(context, NEOROTARY_1X4_BUTTON_C, 0) < 0)
        return SS_ENCODER_C_SETUP_ERROR;
    if (seesaw_encoder_set_position(context, NEOROTARY_1X4_BUTTON_D, 0) < 0)
        return SS_ENCODER_D_SETUP_ERROR;
    if (neo_setup(context, NEOROTARY_1X4_LEDS, NEOROTARY_1X4_LED_PIN) < 0)
        return SS_NEO_SETUP_ERROR;
    return SS_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, NEOROTARY_1X4_I2C_ADDR);

    result = i2c_open(&g_context, argc, argv);
    if (result < 0)
        return result;

    result = setup(&g_context);
    if (result < 0)
        printf("ERROR: setup() failed\n");
    else
        loop(&g_context);

    i2c_close(&g_context);
    return result;
}
