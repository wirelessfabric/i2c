// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Bosch Sensortec BME280 Combined Temperature, Humidity, and Pressure Sensor
// Data Sheet:
// https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf

#ifndef BME280_I2C_H
#define BME280_I2C_H

#include "i2c_io.h"

// BME280 I2C device address
#define BME280_I2C_ADDR 0x77

// BME280 registers
#define BME280_REG_T1               0x88
#define BME280_REG_T2               0x8A
#define BME280_REG_T3               0x8C
#define BME280_REG_P1               0x8E
#define BME280_REG_P2               0x90
#define BME280_REG_P3               0x92
#define BME280_REG_P4               0x94
#define BME280_REG_P5               0x96
#define BME280_REG_P6               0x98
#define BME280_REG_P7               0x9A
#define BME280_REG_P8               0x9C
#define BME280_REG_P9               0x9E
#define BME280_REG_H1               0xA1
#define BME280_REG_H2               0xE1
#define BME280_REG_H3               0xE3
#define BME280_REG_E4               0xE4
#define BME280_REG_E5               0xE5
#define BME280_REG_E6               0xE6
#define BME280_REG_H6               0xE7

#define BME280_REG_CHIP_ID          0xD0
#define BME280_REG_SOFT_RESET       0xE0
#define BME280_REG_CONTROL_HUMIDITY 0xF2
#define BME280_REG_STATUS           0xF3
#define BME280_REG_CONTROL          0xF4
#define BME280_REG_CONFIG           0xF5
#define BME280_REG_PRESSURE         0xF7
#define BME280_REG_TEMPERATURE      0xFA
#define BME280_REG_HUMIDITY         0xFD

// chip id value
#define BME280_CHIP_ID              0x60

// status values
#define BME280_STATUS_IM_UPDATE     0x01
#define BME280_STATUS_MEASURING     0x08

// mode values
#define BME280_MODE_SLEEP           0x00
#define BME280_MODE_FORCE           0x01
#define BME280_MODE_NORMAL          0x03

// overscan values
#define BME280_OVERSCAN_NONE        0x00
#define BME280_OVERSCAN_X1          0x01
#define BME280_OVERSCAN_X2          0x02
#define BME280_OVERSCAN_X4          0x03
#define BME280_OVERSCAN_X8          0x04
#define BME280_OVERSCAN_X16         0x05

#define BME280_OVERSCAN_HUMIDITY_SHIFT 0
#define BME280_OVERSCAN_HUMIDITY    (BME280_OVERSCAN_X1 << BME280_OVERSCAN_HUMIDITY_SHIFT)

#define BME280_OVERSCAN_PRESSURE_SHIFT 2
#define BME280_OVERSCAN_PRESSURE    (BME280_OVERSCAN_X1  << BME280_OVERSCAN_PRESSURE_SHIFT)
#define BME280_OVERSCAN_PRESSURE_16 (BME280_OVERSCAN_X16 << BME280_OVERSCAN_PRESSURE_SHIFT)

#define BME280_OVERSCAN_TEMPERATURE_SHIFT 5
#define BME280_OVERSCAN_TEMPERATURE (BME280_OVERSCAN_X1 << BME280_OVERSCAN_TEMPERATURE_SHIFT)

// filter values
#define BME280_IIR_FILTER_SHIFT     5
#define BME280_IIR_FILTER_OFF       (0x00 << BME280_IIR_FILTER_SHIFT)
#define BME280_IIR_FILTER_X2        (0x01 << BME280_IIR_FILTER_SHIFT)
#define BME280_IIR_FILTER_X4        (0x02 << BME280_IIR_FILTER_SHIFT)
#define BME280_IIR_FILTER_X8        (0x03 << BME280_IIR_FILTER_SHIFT)
#define BME280_IIR_FILTER_X16       (0x04 << BME280_IIR_FILTER_SHIFT)

// standby values
#define BME280_STANDBY_SHIFT        2
#define BME280_STANDBY_MS_0_5       (0x00 << BME280_STANDBY_SHIFT)
#define BME280_STANDBY_MS_62_5      (0x01 << BME280_STANDBY_SHIFT)
#define BME280_STANDBY_MS_125       (0x02 << BME280_STANDBY_SHIFT)
#define BME280_STANDBY_MS_250       (0x03 << BME280_STANDBY_SHIFT)
#define BME280_STANDBY_MS_500       (0x04 << BME280_STANDBY_SHIFT)
#define BME280_STANDBY_MS_1000      (0x05 << BME280_STANDBY_SHIFT)
#define BME280_STANDBY_MS_10        (0x06 << BME280_STANDBY_SHIFT)
#define BME280_STANDBY_MS_20        (0x07 << BME280_STANDBY_SHIFT)

// SPI 3-wire values
#define BME280_SPI_3WIRE_SHIFT      0
#define BME280_SPI_3WIRE_OFF        (0x00 << BME280_SPI_3WIRE_SHIFT)
#define BME280_SPI_3WIRE_ON         (0x01 << BME280_SPI_3WIRE_SHIFT)

// BME280 error codes
#define BME280_SUCCESS              I2C_SUCCESS
#define BME280_NO_RESPONSE          I2C_USER_ERROR(-1)
#define BME280_READ_CHIP_ID_ERROR   I2C_USER_ERROR(-2)
#define BME280_INVALID_CHIP_ID      I2C_USER_ERROR(-3)
#define BME280_RESET_ERROR          I2C_USER_ERROR(-4)

#endif // BME280_I2C_H
