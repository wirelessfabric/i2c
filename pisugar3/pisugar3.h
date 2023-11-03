// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// PiSugar3 
// Data Sheet:
// https://github.com/PiSugar/PiSugar/wiki/PiSugar-3-Series

#ifndef PISUGAR3_I2C_H
#define PISUGAR3_I2C_H

#include "i2c_io.h"

// PiSugar3 I2C device address
#define PISUGAR_I2C_ADDR            0x57

// PiSugar3 I2C registers
#define PISUGAR_VERSION             0x00
#define PISUGAR_MODEL               0x01
#define PISUGAR_CONTROL1            0x02
#define PISUGAR_CONTROL2            0x03
#define PISUGAR_CHIP_TEMPERATURE    0x04
#define PISUGAR_TAP                 0x08
#define PISUGAR_BATTERY_VOLTAGE     0x22
#define PISUGAR_BATTERY_PERCENTAGE  0x2A

// version register values
#define PISUGAR3_VERSION            0x03

// model register values
#define PISUGAR3_MODEL              0x0F

// control register 1 values
#define CONTROL1_POWER_BUTTON       0x01
#define CONTROL1_RESERVED           0x06
#define CONTROL1_TOUCH_FUNCTION     0x08
#define CONTROL1_POWER_ON_CONNECT   0x10
#define CONTROL1_OUTPUT_SWITCH      0x20
#define CONTROL1_CHARGING_SWITCH    0x40
#define CONTROL1_POWER_SUPPLY       0x80

// control register 2 values
#define CONTROL2_SOFT_SHUTDOWN_SIGN 0x04
#define CONTROL2_SOFT_SHUTDOWN      0x08
#define CONTROL2_AUTO_HIBERNATE     0x40

// tap register values
#define TAP_CUSTOM_BUTTON           0x07
#define TAP_RESERVED                0xf8

// EMC2101 error codes
#define PISUGAR_SUCCESS             I2C_SUCCESS
#define PISUGAR_GET_VERSION_ERROR   I2C_USER_ERROR(-1)
#define PISUGAR_INVALID_VERSION     I2C_USER_ERROR(-2)
#define PISUGAR_GET_MODEL_ERROR     I2C_USER_ERROR(-3)
#define PISUGAR_INVALID_MODEL       I2C_USER_ERROR(-4)

#endif // PISUGAR3_I2C_H
