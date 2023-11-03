// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Seesaw NeoPixel Module
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/seesaw_neopixel.h
//

#ifndef SEESAW_NEOPIXEL_H_
#define SEESAW_NEOPIXEL_H_

#include "adafruit_seesaw.h"

// Seesaw NeoPixel module function register addresses
#define SS_NEOPIXEL_STATUS      0x00
#define SS_NEOPIXEL_PIN         0x01
#define SS_NEOPIXEL_SPEED       0x02
#define SS_NEOPIXEL_BUFLEN      0x03
#define SS_NEOPIXEL_BUF         0x04
#define SS_NEOPIXEL_SHOW        0x05

// Seesaw NeoPixel module speed definitions
#define SS_NEOPIXEL_400KHZ      0x00
#define SS_NEOPIXEL_800KHZ      0x01

// Rotary Encoder with NeoPixel product definitions
#define NEOROTARY_LEDS          1
#define NEOROTARY_LED_PIN       6

// Quad Rotary Encoder with NeoPixel product definitions
#define NEOROTARY_1X4_LEDS      4
#define NEOROTARY_1X4_LED_PIN   18

// NeoKey 1x4 product definitions
#define NEOKEY_1X4_LEDS         4
#define NEOKEY_1X4_LED_PIN      3

// NeoSlider product definitions
#define NEOSLIDER_LEDS          4
#define NEOSLIDER_LED_PIN       14

int neo_get_led_count(void);
void neo_set_led_count(int value);

uint16_t neo_get_brightness(void);
void neo_set_brightness(uint16_t value);
uint8_t neo_apply_brightness(uint8_t color);

int neo_set_pin(void* context, uint8_t pin);
int neo_set_speed(void* context, uint8_t speed);
int neo_set_buflen(void* context, int leds);

int neo_set_led(void* context, int led, uint8_t r, uint8_t g, uint8_t b);
int neo_set_led_black(void* context, int led);
int neo_set_led_red(void* context, int led);
int neo_set_led_green(void* context, int led);
int neo_set_led_blue(void* context, int led);
int neo_set_led_white(void* context, int led);

int neo_set_all(void* context, uint8_t r, uint8_t g, uint8_t b);

int neo_show(void* context);

int neo_show_all(void* context, uint8_t r, uint8_t g, uint8_t b);
int neo_show_black(void* context);
int neo_show_green(void* context);
int neo_show_red(void* context);
int neo_show_blue(void* context);
int neo_show_white(void* context);
int neo_show_color(void* context, uint8_t color);

int neo_setup(void* context, int leds, int pin);

#endif // SEESAW_NEOPIXEL_H_
