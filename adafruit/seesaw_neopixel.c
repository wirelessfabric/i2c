// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Seesaw NeoPixel Module
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/seesaw_neopixel.cpp
//

#include <stdio.h>
#include <stdlib.h>
#include "seesaw_neopixel.h"
#include "seesaw_gpio.h"
#include "i2c_io.h"

#define NEO_MIN_LEDS        1
#define NEO_MAX_LEDS        4

#define NEO_MIN_BRIGHTNESS  20
#define NEO_MAX_BRIGHTNESS  255

// green, red, and blue color channels
#define NEO_GRB_COLORS      3
#define NEO_MAX_BUFLEN      (NEO_MAX_LEDS * NEO_GRB_COLORS)

static int neo_led_count = 1;
static uint16_t neo_brightness = 255;

int neo_get_led_count(void) {
    return neo_led_count;
}

void neo_set_led_count(int value) {
    if (value < NEO_MIN_LEDS)
        value = NEO_MIN_LEDS;
    else if (value > NEO_MAX_LEDS)
        value = NEO_MAX_LEDS;
    neo_led_count = value;
}

uint16_t neo_get_brightness(void) {
    return neo_brightness;
}

void neo_set_brightness(uint16_t value) {
    if (value < NEO_MIN_BRIGHTNESS)
        value = NEO_MIN_BRIGHTNESS;
    else if (value > NEO_MAX_BRIGHTNESS)
        value = NEO_MAX_BRIGHTNESS;
    neo_brightness = value;
}

uint8_t neo_apply_brightness(uint8_t color) {
    return (color * neo_brightness) >> 8;
}

int neo_set_pin(void* context, uint8_t pin) {
    return i2c_write_module_u8(context, SS_NEOPIXEL, SS_NEOPIXEL_PIN, pin);
}

int neo_set_speed(void* context, uint8_t speed) {
    if (speed > SS_NEOPIXEL_800KHZ)
        speed = SS_NEOPIXEL_800KHZ;
    return i2c_write_module_u8(context, SS_NEOPIXEL, SS_NEOPIXEL_SPEED, speed);
}

int neo_set_buflen(void* context, int leds) {
    neo_set_led_count(leds);
    uint16_t buflen = (uint16_t)(neo_get_led_count() * NEO_GRB_COLORS);
    uint8_t command[4] = {
        (uint8_t)SS_NEOPIXEL,
        (uint8_t)SS_NEOPIXEL_BUFLEN,
        (uint8_t)(buflen >> 8),
        (uint8_t)(buflen & 0xFF)
    };
    return i2c_write(context, command, sizeof(command));
}

int neo_set_led(void* context, int led, uint8_t r, uint8_t g, uint8_t b) {
    if (led < 0 || led >= neo_get_led_count())
        return SS_NEO_INVALID_LED;

    uint16_t offset = (uint16_t)(led * NEO_GRB_COLORS);
    uint8_t command[7];
    command[0] = (uint8_t)SS_NEOPIXEL,
    command[1] = (uint8_t)SS_NEOPIXEL_BUF,
    command[2] = (uint8_t)(offset >> 8);
    command[3] = (uint8_t)(offset & 0xFF);
    command[4] = neo_apply_brightness(g);
    command[5] = neo_apply_brightness(r);
    command[6] = neo_apply_brightness(b);
    return i2c_write(context, command, sizeof(command));
}

int neo_set_led_black(void* context, int led) {
    return neo_set_led(context, led, 0, 0, 0);
}

int neo_set_led_red(void* context, int led) {
    return neo_set_led(context, led, 255, 0, 0);
}

int neo_set_led_green(void* context, int led) {
    return neo_set_led(context, led, 0, 255, 0);
}

int neo_set_led_blue(void* context, int led) {
    return neo_set_led(context, led, 0, 0, 255);
}

int neo_set_led_white(void* context, int led) {
    return neo_set_led(context, led, 255, 255, 255);
}

int neo_set_all(void* context, uint8_t r, uint8_t g, uint8_t b) {
    r = neo_apply_brightness(r);
    g = neo_apply_brightness(g);
    b = neo_apply_brightness(b);

    uint8_t command[4 + NEO_MAX_BUFLEN];
    command[0] = (uint8_t)SS_NEOPIXEL,
    command[1] = (uint8_t)SS_NEOPIXEL_BUF,
    command[2] = 0;
    command[3] = 0;
    command[4] = g;
    command[5] = r;
    command[6] = b;
    size_t size = 7;

    int leds = neo_get_led_count();
    for (int led=1; led < leds; led++) {
        command[size++] = g;
        command[size++] = r;
        command[size++] = b;
    }

    return i2c_write(context, command, size);
}

int neo_show(void* context) {
    uint8_t command[2] = {
        (uint8_t)SS_NEOPIXEL,
        (uint8_t)SS_NEOPIXEL_SHOW
    };
    i2c_sleep(1000);
    return i2c_write(context, command, sizeof(command));
}

int neo_show_all(void* context, uint8_t r, uint8_t g, uint8_t b) {
    return neo_set_all(context, r, g, b) == 0 ? neo_show(context) : -1;
}

int neo_show_black(void* context) {
    return neo_show_all(context, 0, 0, 0);
}

int neo_show_red(void* context) {
    return neo_show_all(context, 255, 0, 0);
}

int neo_show_green(void* context) {
    return neo_show_all(context, 0, 255, 0);
}

int neo_show_blue(void* context) {
    return neo_show_all(context, 0, 0, 255);
}

int neo_show_white(void* context) {
    return neo_show_all(context, 255, 255, 255);
}

int neo_show_color(void* context, uint8_t color) {
    uint8_t r = 0, g, b = 0;

    if (color < 85) {
        g = color * 3;
        r = 255 - g;
    } else if (color < 170) {
        g = (color - 85) * 3;
        b = 255 - g;
    } else {
        r = (color - 170) * 3;
        g = 255 - r;
    }

    return neo_show_all(context, r, g, b);
}

int neo_setup(void* context, int leds, int pin) {
    if (neo_set_speed(context, SS_NEOPIXEL_800KHZ) < 0)
        return SS_NEO_SET_SPEED_ERROR;
    if (neo_set_buflen(context, leds) < 0)
        return SS_NEO_SET_BUFLEN_ERROR;
    if (neo_set_pin(context, pin) < 0)
        return SS_NEO_SET_PIN_ERROR;

    // turn all off
    if (neo_show_black(context) < 0)
        return SS_NEO_SHOW_BLACK_ERROR;

    return 0;
}
