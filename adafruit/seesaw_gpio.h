// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Seesaw GPIO Module
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/Adafruit_seesaw.h
//

#ifndef SEESAW_GPIO_H_
#define SEESAW_GPIO_H_

#include "adafruit_seesaw.h"

// GPIO module function address registers
#define SS_GPIO_DIRSET_BULK         0x02
#define SS_GPIO_DIRCLR_BULK         0x03
#define SS_GPIO_BULK                0x04
#define SS_GPIO_BULK_SET            0x05
#define SS_GPIO_BULK_CLR            0x06
#define SS_GPIO_BULK_TOGGLE         0x07
#define SS_GPIO_INTENSET            0x08
#define SS_GPIO_INTENCLR            0x09
#define SS_GPIO_INTFLAG             0x0A
#define SS_GPIO_PULLENSET           0x0B
#define SS_GPIO_PULLENCLR           0x0C

// GPIO pin modes
#define SS_GPIO_INPUT               0x00
#define SS_GPIO_OUTPUT              0x01
#define SS_GPIO_INPUT_PULLUP        0x02
#define SS_GPIO_INPUT_PULLDOWN      0x03

// GPIO pin bitmask
#define SS_GPIO_PIN_MASK(pin)       ((uint32_t)1 << (uint32_t)(pin))

// Rotary Encoder Breakout with NeoPixel product definitions
#define NEOROTARY_BUTTON            0
#define NEOROTARY_BUTTON_PIN        SS_GPIO_PIN_MASK(24)

// Quad Rotary Encoder Breakout with NeoPixel product definitions
#define NEOROTARY_1X4_BUTTON_A      0
#define NEOROTARY_1X4_BUTTON_A_PIN  SS_GPIO_PIN_MASK(12)
#define NEOROTARY_1X4_BUTTON_B      1
#define NEOROTARY_1X4_BUTTON_B_PIN  SS_GPIO_PIN_MASK(14)
#define NEOROTARY_1X4_BUTTON_C      2
#define NEOROTARY_1X4_BUTTON_C_PIN  SS_GPIO_PIN_MASK(17)
#define NEOROTARY_1X4_BUTTON_D      3
#define NEOROTARY_1X4_BUTTON_D_PIN  SS_GPIO_PIN_MASK(9)
#define NEOROTARY_1X4_BUTTON_PINS   (NEOROTARY_1X4_BUTTON_A_PIN | \
                                     NEOROTARY_1X4_BUTTON_B_PIN | \
                                     NEOROTARY_1X4_BUTTON_C_PIN | \
                                     NEOROTARY_1X4_BUTTON_D_PIN)

// NeoKey 1x4 product definitions
#define NEOKEY_1X4_BUTTON_A         0
#define NEOKEY_1X4_BUTTON_A_PIN     SS_GPIO_PIN_MASK(4)
#define NEOKEY_1X4_BUTTON_B         1
#define NEOKEY_1X4_BUTTON_B_PIN     SS_GPIO_PIN_MASK(5)
#define NEOKEY_1X4_BUTTON_C         2
#define NEOKEY_1X4_BUTTON_C_PIN     SS_GPIO_PIN_MASK(6)
#define NEOKEY_1X4_BUTTON_D         3
#define NEOKEY_1X4_BUTTON_D_PIN     SS_GPIO_PIN_MASK(7)
#define NEOKEY_1X4_BUTTON_PINS      (NEOKEY_1X4_BUTTON_A_PIN | \
                                     NEOKEY_1X4_BUTTON_B_PIN | \
                                     NEOKEY_1X4_BUTTON_C_PIN | \
                                     NEOKEY_1X4_BUTTON_D_PIN)

int seesaw_gpio_set_mode(void* context, uint32_t bulk_mask, uint8_t mode);
int seesaw_gpio_set_interrupt(void* context, uint32_t bulk_mask, bool enable);
int seesaw_gpio_get_bulk(void* context, uint32_t bulk_mask, uint32_t* value);

int seesaw_gpio_setup(void* context, uint32_t bulk_mask, uint8_t mode);

#endif // SEESAW_GPIO_H_
