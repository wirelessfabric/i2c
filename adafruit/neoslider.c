// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT NeoSlider Slide Potentiometer with 4 NeoPixels
// https://learn.adafruit.com/adafruit-i2c-qt-rotary-encoder
// https://www.adafruit.com/product/5295
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/examples/NeoSlider/NeoSlider/NeoSlider.ino
//
// $ gcc neoslider.c
//       adafruit_seesaw.c
//       seesaw_gpio.c
//       seesaw_adc.c
//       seesaw_neopixel.c
//       ../io/i2c_dev_io.c -I../io
//       -o neoslider
// $ ./neoslider

#include <stdio.h>
#include <stdlib.h>
#include "seesaw_gpio.h"
#include "seesaw_adc.h"
#include "seesaw_neopixel.h"
#include "i2c_io.h"

static void loop(void* context) {
    uint8_t color;
    uint16_t slider_value = -1;

    while (true) {
        uint16_t value = 0;
        if (seesaw_adc_get_channel(context, NEOSLIDER_ADC_CHANNEL, &value) == 0) {
            // potentiometer value range: [0 to 1023]
            value &= 1023;
            if (slider_value != value) {
                slider_value = value;
                printf("Slider: 0x%04hx (%d)\n", slider_value, slider_value);
                color = (uint8_t)(uint32_t)((float)value / 1023.f * 255.f);
                neo_show_color(context, color);
            }
        }

        i2c_sleep(1000);
    }
}

static int setup(void* context) {
    if (seesaw_tiny8x7_setup(context, NEOSLIDER_FW_VERSION, NEOSLIDER_FW_OPTIONS) < 0)
        return SS_TINY8X7_SETUP_ERROR;
    if (seesaw_adc_setup(context) < 0)
        return SS_ADC_SETUP_ERROR;
    if (neo_setup(context, NEOSLIDER_LEDS, NEOSLIDER_LED_PIN) < 0)
        return SS_NEO_SETUP_ERROR;
    return SS_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, NEOSLIDER_I2C_ADDR);

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
