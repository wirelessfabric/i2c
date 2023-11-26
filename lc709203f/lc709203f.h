// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit LC709203F LiPoly / LiIon Fuel Gauge and Battery Monitor
// Smart LiB Gauge Battery Fuel Gauge LSI For 1-Cell Lithium-ion/Polymer (Li+)
// Data Sheet:
// See: https://cdn-learn.adafruit.com/assets/assets/000/094/597/original/LC709203F-D.PDF?1599248750

#ifndef LC709203F_I2C_H
#define LC709203F_I2C_H

#include "i2c_io.h"

// LC709203F I2C device address
#define LC709203F_I2C_ADDR 0x0b

// LC709203F registers
#define LC709203F_REG_THERMISTOR_B      0x06
#define LC709203F_REG_INIT_RSOC         0x07
#define LC709203F_REG_CELL_TEMPERATURE  0x08
#define LC709203F_REG_CELL_VOLTAGE      0x09
#define LC709203F_REG_CURRENT_FLOW_MODE 0x0A
#define LC709203F_REG_APA               0x0B
#define LC709203F_REG_CELL_ITE          0x0F
#define LC709203F_REG_IC_VERSION        0x11
#define LC709203F_REG_BATTERY_PROFILE   0x12
#define LC709203F_REG_ALARM_PERCENT     0x13
#define LC709203F_REG_ALARM_VOLTAGE     0x14
#define LC709203F_REG_POWER_MODE        0x15
#define LC709203F_REG_STATUS_BIT        0x16

// LC709203F IC version
#define LC709203F_IC_VERSION            0x2AFE

// LC709203F current flow modes
#define LC709203F_FLOW_AUTO             0x0000
#define LC709203F_FLOW_CHARGE           0x0001
#define LC709203F_FLOW_DISCHARGE        0xFFFF

// LC709203F power modes
#define LC709203F_POWER_OPERATIONAL     0x0001
#define LC709203F_POWER_SLEEP           0x0002

// LC709203F adjustment Pack application
#define LC709203F_APA_100MAH            0x0008
#define LC709203F_APA_200MAH            0x000B
#define LC709203F_APA_500MAH            0x0010
#define LC709203F_APA_1000MAH           0x0019
#define LC709203F_APA_2000MAH           0x002D
#define LC709203F_APA_3000MAH           0x0036

// LC709203F battery profile
#define LC709203F_BATTERY_4700mV        0x0001

// thermistor B constant
#define LC709203F_THERMISTOR_B          3950

// LC709203F status bit
#define LC709203F_I2C_MODE              0x0000
#define LC709203F_THERMISTOR_MODE       0x0001

// LC709203F cell temperature in 0.1C (or deciKelvin) units
#define LC709203F_TEMPERATURE_MIN_dK    2532.f  // 0x09E4 = -20C
#define LC709203F_TEMPERATURE_ZERO_dK   2731.5f // 0 Kelvin
#define LC709203F_TEMPERATURE_MAX_dK    3332.f  // 0x0D04 = +60C
#define LC709203F_TEMPERATURE_MIN_C     -20.f   // -20C = 0x09E4
#define LC709203F_TEMPERATURE_ZERO_C    0.f     // 0 Celsius
#define LC709203F_TEMPERATURE_MAX_C     60.f    // +60C = 0x0D04
#define LC709203F_TEMPERATURE_RANGE_dK  (LC709203F_TEMPERATURE_MAX_dK - LC709203F_TEMPERATURE_MIN_dK)
#define LC709203F_TEMPERATURE_RANGE_C   (LC709203F_TEMPERATURE_MAX_C - LC709203F_TEMPERATURE_MIN_C)

// LC709203F error codes
#define LC709203F_SUCCESS                       I2C_SUCCESS
#define LC709203F_NO_RESPONSE                   I2C_USER_ERROR(-1)
#define LC709203F_INVALID_IC_VERSION            I2C_USER_ERROR(-2)
#define LC709203F_SET_POWER_MODE_ERROR          I2C_USER_ERROR(-3)
#define LC709203F_SET_APA_ERROR                 I2C_USER_ERROR(-4)
#define LC709203F_SET_BATTERY_PROFILE_ERROR     I2C_USER_ERROR(-5)
#define LC709203F_SET_THERMISTOR_B_ERROR        I2C_USER_ERROR(-6)
#define LC709203F_SET_THERMISTOR_MODE_ERROR     I2C_USER_ERROR(-7)

#endif // LC709203F_I2C_H
