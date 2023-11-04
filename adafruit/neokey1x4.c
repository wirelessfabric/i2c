// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT NeoKey 1x4 - Four Mechanical Key Switches with NeoPixels
// https://learn.adafruit.com/neokey-1x4-qt-i2c
// https://www.adafruit.com/product/4980
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/examples/NeoKey_1x4/basic_neoswirl/basic_neoswirl.ino
//
// $ gcc neokey1x4.c
//       adafruit_seesaw.c
//       seesaw_gpio.c
//       seesaw_adc.c
//       seesaw_neopixel.c
//       ../io/i2c_dev_io.c -I../io
//       -o neokey1x4
// $ ./neokey1x4

#include <stdio.h>
#include <stdlib.h>
#include "seesaw_gpio.h"
#include "seesaw_neopixel.h"
#include "i2c_io.h"

static void loop(void* context) {
    uint32_t gpio_pin[4] = {
        NEOKEY_1X4_BUTTON_A_PIN,
        NEOKEY_1X4_BUTTON_B_PIN,
        NEOKEY_1X4_BUTTON_C_PIN,
        NEOKEY_1X4_BUTTON_D_PIN
    };
    const char* button_name[4] = { "A", "B", "C", "D" };
    uint32_t button_state[4] = { 0, 0, 0, 0 };
    uint32_t bulk_state = 0;

    while (true) {
        uint32_t bulk = 0;
        if (seesaw_gpio_get_bulk(context, NEOKEY_1X4_BUTTON_PINS, &bulk) == 0) {
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
    if (seesaw_samd09_setup(context, NEOKEY_1X4_FW_VERSION, NEOKEY_1X4_FW_OPTIONS) < 0)
        return -1;
    if (seesaw_gpio_setup(context, NEOKEY_1X4_BUTTON_PINS, SS_GPIO_INPUT_PULLUP) < 0)
        return -2;
    if (neo_setup(context, NEOKEY_1X4_LEDS, NEOKEY_1X4_LED_PIN) < 0)
        return -3;
    return neo_show_black(context);
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, NEOKEY_1X4_I2C_ADDR);

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
