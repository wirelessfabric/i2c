// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Bosch Sensortec BME680/BME688 Combined Temperature, Humidity, Pressure, and Gas Sensor
// Data Sheet:
// https://cdn-shop.adafruit.com/product-files/3660/BME680.pdf

#ifndef BME68X_I2C_H
#define BME68X_I2C_H

#include "i2c_io.h"

// BME68X I2C device address
#define BME68X_I2C_ADDR 0x77

// BME68X registers
#define BME68X_REG_STATUS_MEASURE   0x1D
#define BME68X_REG_P1               0x1F
#define BME68X_REG_P2               0x20
#define BME68X_REG_P3               0x21
#define BME68X_REG_T1               0x22
#define BME68X_REG_T2               0x23
#define BME68X_REG_T3               0x24
#define BME68X_REG_H1               0x25
#define BME68X_REG_H2               0x26
#define BME68X_REG_H3               0x27

#define BME68X_REG_CONTROL_GAS      0x71
#define BME68X_REG_CONTROL_HUMIDITY 0x72
#define BME68X_REG_STATUS           0x73
#define BME68X_REG_CONTROL          0x74
#define BME68X_REG_CONFIG           0x75

#define BME68X_REG_P4               0x94
#define BME68X_REG_P5               0x96
#define BME68X_REG_P6               0x98
#define BME68X_REG_P7               0x9A
#define BME68X_REG_P8               0x9C
#define BME68X_REG_P9               0x9E
#define BME68X_REG_E4               0xE4
#define BME68X_REG_E5               0xE5
#define BME68X_REG_E6               0xE6
#define BME68X_REG_H6               0xE7

#define BME68X_REG_CHIP_ID          0xD0
#define BME68X_REG_SOFT_RESET       0xE0
#define BME68X_REG_VARIANT          0xF0
#define BME68X_REG_PRESSURE         0xF7
#define BME68X_REG_TEMPERATURE      0xFA
#define BME68X_REG_HUMIDITY         0xFD

// chip id value
#define BME68X_CHIP_ID              0x61

// variant id value
#define BME68X_VARIANT_GAS_LOW      0x00
#define BME68X_VARIANT_GAS_HI       0x01
#define BME68X_VARIANT_MAX          BME68X_VARIANT_GAS_HI

// status values
#define BME68X_STATUS_IM_UPDATE     0x01
#define BME68X_STATUS_MEASURING     0x80

// mode values
#define BME68X_MODE_SLEEP           0x00
#define BME68X_MODE_FORCE           0x01
#define BME68X_MODE_NORMAL          0x03

// overscan values
#define BME68X_OVERSCAN_NONE        0x00
#define BME68X_OVERSCAN_X1          0x01
#define BME68X_OVERSCAN_X2          0x02
#define BME68X_OVERSCAN_X4          0x03
#define BME68X_OVERSCAN_X8          0x04
#define BME68X_OVERSCAN_X16         0x05

#define BME68X_OVERSCAN_HUMIDITY_SHIFT 0
#define BME68X_OVERSCAN_HUMIDITY    (BME68X_OVERSCAN_X2 << BME68X_OVERSCAN_HUMIDITY_SHIFT)

#define BME68X_OVERSCAN_PRESSURE_SHIFT 2
#define BME68X_OVERSCAN_PRESSURE    (BME68X_OVERSCAN_X4  << BME68X_OVERSCAN_PRESSURE_SHIFT)
#define BME68X_OVERSCAN_PRESSURE_16 (BME68X_OVERSCAN_X16 << BME68X_OVERSCAN_PRESSURE_SHIFT)

#define BME68X_OVERSCAN_TEMPERATURE_SHIFT 5
#define BME68X_OVERSCAN_TEMPERATURE (BME68X_OVERSCAN_X8 << BME68X_OVERSCAN_TEMPERATURE_SHIFT)

// filter values
#define BME68X_IIR_FILTER_SHIFT     5
#define BME68X_IIR_FILTER_OFF       (0x00 << BME68X_IIR_FILTER_SHIFT)
#define BME68X_IIR_FILTER_X2        (0x01 << BME68X_IIR_FILTER_SHIFT)
#define BME68X_IIR_FILTER_X4        (0x02 << BME68X_IIR_FILTER_SHIFT)
#define BME68X_IIR_FILTER_X8        (0x03 << BME68X_IIR_FILTER_SHIFT)
#define BME68X_IIR_FILTER_X16       (0x04 << BME68X_IIR_FILTER_SHIFT)

// standby values
#define BME68X_STANDBY_SHIFT        2
#define BME68X_STANDBY_MS_0_5       (0x00 << BME68X_STANDBY_SHIFT)
#define BME68X_STANDBY_MS_62_5      (0x01 << BME68X_STANDBY_SHIFT)
#define BME68X_STANDBY_MS_125       (0x02 << BME68X_STANDBY_SHIFT)
#define BME68X_STANDBY_MS_250       (0x03 << BME68X_STANDBY_SHIFT)
#define BME68X_STANDBY_MS_500       (0x04 << BME68X_STANDBY_SHIFT)
#define BME68X_STANDBY_MS_1000      (0x05 << BME68X_STANDBY_SHIFT)
#define BME68X_STANDBY_MS_10        (0x06 << BME68X_STANDBY_SHIFT)
#define BME68X_STANDBY_MS_20        (0x07 << BME68X_STANDBY_SHIFT)

// SPI 3-wire values
#define BME68X_SPI_3WIRE_SHIFT      0
#define BME68X_SPI_3WIRE_OFF        (0x00 << BME68X_SPI_3WIRE_SHIFT)
#define BME68X_SPI_3WIRE_ON         (0x01 << BME68X_SPI_3WIRE_SHIFT)

// BME68X error codes
#define BME68X_SUCCESS              I2C_SUCCESS
#define BME68X_NO_RESPONSE          I2C_USER_ERROR(-1)
#define BME68X_READ_CHIP_ID_ERROR   I2C_USER_ERROR(-2)
#define BME68X_INVALID_CHIP_ID      I2C_USER_ERROR(-3)
#define BME68X_READ_VARIANT_ERROR   I2C_USER_ERROR(-4)
#define BME68X_INVALID_VARIANT      I2C_USER_ERROR(-5)
#define BME68X_RESET_ERROR          I2C_USER_ERROR(-6)

#endif // BME68X_I2C_H
