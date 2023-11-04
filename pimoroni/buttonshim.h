// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Pimoroni Button SHIM for Raspberry Pi
// 
// See: https://www.adafruit.com/product/3582
// See: https://pinout.xyz/pinout/button_shim

#ifndef BUTTONSHIM_I2C_H
#define BUTTONSHIM_I2C_H

#include "i2c_io.h"

// button shim I2C device address
#define BUTTONSHIM_I2C_ADDR 0x3f

// button shim registers
#define BUTTONSHIM_REG_INPUT        0x00
#define BUTTONSHIM_REG_OUTPUT       0x01
#define BUTTONSHIM_REG_POLARITY     0x02
#define BUTTONSHIM_REG_CONFIG       0x03

// button shim buttons
#define BUTTONSHIM_BUTTON_A         0
#define BUTTONSHIM_BUTTON_B         1
#define BUTTONSHIM_BUTTON_C         2
#define BUTTONSHIM_BUTTON_D         3
#define BUTTONSHIM_BUTTON_E         4

#define BUTTONSHIM_BUTTON_A_PIN     0
#define BUTTONSHIM_BUTTON_B_PIN     1
#define BUTTONSHIM_BUTTON_C_PIN     2
#define BUTTONSHIM_BUTTON_D_PIN     3
#define BUTTONSHIM_BUTTON_E_PIN     4

// button shim output values
#define BUTTONSHIM_OUTPUT_INIT      0x00

// button shim polarity values
#define BUTTONSHIM_POLARITY_INIT    0x00

// button shim config values
#define BUTTONSHIM_CONFIG_INIT      0x1f

// button shim error codes
#define BUTTONSHIM_SUCCESS              I2C_SUCCESS
#define BUTTONSHIM_SET_CONFIG_ERROR     I2C_USER_ERROR(-1)
#define BUTTONSHIM_SET_POLARITY_ERROR   I2C_USER_ERROR(-2)
#define BUTTONSHIM_SET_OUTPUT_ERROR     I2C_USER_ERROR(-3)

#endif // BUTTONSHIM_I2C_H
