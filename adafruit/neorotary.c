// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Rotary Encoder Breakout with NeoPixel
// https://learn.adafruit.com/adafruit-i2c-qt-rotary-encoder
// https://www.adafruit.com/product/4991
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/examples/encoder/encoder_basic/encoder_basic.ino
//
// $ gcc neorotary.c
//       adafruit_seesaw.c
//       seesaw_gpio.c
//       seesaw_encoder.c
//       seesaw_neopixel.c
//       ../io/i2c_dev_io.c -I../io
//       -o neorotary
// $ ./neorotary

#include <stdio.h>
#include <stdlib.h>
#include "seesaw_gpio.h"
#include "seesaw_encoder.h"
#include "seesaw_neopixel.h"
#include "i2c_io.h"

static void loop(void* context) {
    uint8_t color = 0;
    uint16_t dimmer = 255;
    uint32_t button_state = 0;
    int32_t encoder_position = -1;

    while (true) {
        int32_t position = 0;
        if (seesaw_encoder_get_position(context, NEOROTARY_BUTTON, &position) == 0) {
            // negate to make clockwise rotation positive
            position = -position; 
            if (encoder_position != position) {
                encoder_position = position;
                printf("Position: %d\n", encoder_position);
                color = encoder_position & 0xff;
                neo_show_color(context, color);
            }
        }

        uint32_t state = 0;
        if (seesaw_gpio_get_bulk(context, NEOROTARY_BUTTON_PIN, &state) == 0) {
            if (button_state != state) {
                button_state = state;
                if (button_state == 0) {
                    printf("Button: Pressed\n");
                    dimmer = dimmer == 255 ? 20 : 255;
                    neo_set_brightness(dimmer);
                    neo_show_color(context, color);
                }
                else
                    printf("Button: Released\n");
            }
        }

        i2c_sleep(5000);
    }
}

static int setup(void* context) {
    if (seesaw_samd09_setup(context, NEOROTARY_FW_VERSION, NEOROTARY_FW_OPTIONS) < 0)
        return SS_SAMD09_SETUP_ERROR;
    if (seesaw_gpio_setup(context, NEOROTARY_BUTTON_PIN, SS_GPIO_INPUT_PULLUP) < 0)
        return SS_GPIO_SETUP_ERROR;
    if (seesaw_encoder_set_position(context, NEOROTARY_BUTTON, 0) < 0)
        return SS_ENCODER_SETUP_ERROR;
    if (neo_setup(context, NEOROTARY_LEDS, NEOROTARY_LED_PIN) < 0)
        return SS_NEO_SETUP_ERROR;
    return SS_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, NEOROTARY_I2C_ADDR);

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
