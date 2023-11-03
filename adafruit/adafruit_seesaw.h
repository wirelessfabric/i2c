// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit Seesaw Platform
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/Adafruit_seesaw.h
//

#ifndef ADAFRUIT_SEESAW_H_
#define ADAFRUIT_SEESAW_H_

#include <stdint.h>
#include <stdbool.h>
#include "i2c_io.h"

// Seesaw module base addresses
#define SS_STATUS                   0x00
#define SS_GPIO                     0x01
#define SS_TIMER                    0x08
#define SS_NEOPIXEL                 0x0E
#define SS_ADC                      0x09
#define SS_ENCODER                  0x11

// Seesaw status module function register addresses
#define SS_STATUS_HW_ID             0x01
#define SS_STATUS_FW_VERSION        0x02
#define SS_STATUS_FW_OPTIONS        0x03
#define SS_STATUS_TEMPERATURE       0x04
#define SS_STATUS_SW_RESET          0x7F

// Seesaw Atmel SAMD09 MCU values
#define SS_SAMD09_HW_ID             0x55

// Seesaw Atmel Tiny8X7 MCU values
#define SS_TINY8X7_HW_ID            0x87

// Seesaw firmware option values
#define SS_FW_OPTION(base)          ((uint32_t)1 << (uint32_t)(base))

// Seesaw error codes
#define SS_SUCCESS                  I2C_SUCCESS
#define SS_GET_TEMPERATURE_ERROR    I2C_USER_ERROR(-1)
#define SS_GET_HW_ID_ERROR          I2C_USER_ERROR(-2)
#define SS_INVALID_HW_ID            I2C_USER_ERROR(-3)
#define SS_GET_FW_VERSION_ERROR     I2C_USER_ERROR(-4)
#define SS_INVALID_FW_VERSION       I2C_USER_ERROR(-5)
#define SS_GET_FW_OPTIONS_ERROR     I2C_USER_ERROR(-6)
#define SS_NO_FW_VERSION            I2C_USER_ERROR(-7)
#define SS_ADC_GET_STATUS_ERROR     I2C_USER_ERROR(-8)
#define SS_ADC_SETUP_ERROR          I2C_USER_ERROR(-8)
#define SS_GPIO_INVALID_PIN_MODE    I2C_USER_ERROR(-9)
#define SS_GPIO_DIRCLR_BULK_ERROR   I2C_USER_ERROR(-10)
#define SS_GPIO_PULLENSET_ERROR     I2C_USER_ERROR(-11)
#define SS_GPIO_SETUP_ERROR         I2C_USER_ERROR(-12)
#define SS_NEO_INVALID_LED          I2C_USER_ERROR(-13)
#define SS_NEO_SET_SPEED_ERROR      I2C_USER_ERROR(-14)
#define SS_NEO_SET_BUFLEN_ERROR     I2C_USER_ERROR(-15)
#define SS_NEO_SET_PIN_ERROR        I2C_USER_ERROR(-16)
#define SS_NEO_SHOW_BLACK_ERROR     I2C_USER_ERROR(-17)
#define SS_NEO_SETUP_ERROR          I2C_USER_ERROR(-18)
#define SS_ENCODER_SETUP_ERROR      I2C_USER_ERROR(-20)
#define SS_ENCODER_A_SETUP_ERROR    I2C_USER_ERROR(-21)
#define SS_ENCODER_B_SETUP_ERROR    I2C_USER_ERROR(-22)
#define SS_ENCODER_C_SETUP_ERROR    I2C_USER_ERROR(-23)
#define SS_ENCODER_D_SETUP_ERROR    I2C_USER_ERROR(-24)
#define SS_TINY8X7_SETUP_ERROR      I2C_USER_ERROR(-19)
#define SS_SAMD09_SETUP_ERROR       I2C_USER_ERROR(-1)

// Rotary Encoder Breakout with NeoPixel product definitions
#define NEOROTARY_I2C_ADDR          0x36
#define NEOROTARY_FW_VERSION        0x137f
#define NEOROTARY_FW_OPTIONS        (SS_FW_OPTION(SS_STATUS) | \
                                     SS_FW_OPTION(SS_NEOPIXEL) | \
                                     SS_FW_OPTION(SS_GPIO) | \
                                     SS_FW_OPTION(SS_ENCODER))

// Quad Rotary Encoder Breakout with NeoPixel product definitions
#define NEOROTARY_1X4_I2C_ADDR      0x49
#define NEOROTARY_1X4_FW_VERSION    0x1678
#define NEOROTARY_1X4_FW_OPTIONS    0

// NeoKey 1x4 product definitions
#define NEOKEY_1X4_I2C_ADDR         0x30
#define NEOKEY_1X4_FW_VERSION       0x1374
#define NEOKEY_1X4_FW_OPTIONS       (SS_FW_OPTION(SS_STATUS) | \
                                     SS_FW_OPTION(SS_GPIO) | \
                                     SS_FW_OPTION(SS_NEOPIXEL))

// NeoSlider product definitions
#define NEOSLIDER_I2C_ADDR          0x30
#define NEOSLIDER_FW_VERSION        0
#define NEOSLIDER_FW_OPTIONS        (SS_FW_OPTION(SS_STATUS) | \
                                     SS_FW_OPTION(SS_NEOPIXEL) | \
                                     SS_FW_OPTION(SS_ADC))

///////////////////////////////////////////////////////////////////////////////

int seesaw_read_u8(context* context, int base, int reg, uint8_t* value);
int seesaw_read_u16_be(context* context, int base, int reg, uint16_t* value);
int seesaw_read_u32_be(context* context, int base, int reg, uint32_t* value);
int seesaw_read_s32_be(context* context, int base, int reg, int32_t* value);

///////////////////////////////////////////////////////////////////////////////

int seesaw_sw_reset(void* context);
int seesaw_get_hw_id(void* context, uint8_t* hw_id);
int seesaw_get_fw_version(void* context, uint32_t* fw_version);
int seesaw_get_fw_options(void* context, uint32_t* fw_options);
int seesaw_get_temperature(void* context, float* temperature);
int seesaw_samd09_setup(void* context, uint32_t version, uint32_t options);
int seesaw_tiny8x7_setup(void* context, uint32_t version, uint32_t options);

///////////////////////////////////////////////////////////////////////////////

#endif // ADAFRUIT_SEESAW_H_
