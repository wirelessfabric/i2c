// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit EMC2101 Fan Controller and Temperature Sensor
// Data Sheet:
// https://ww1.microchip.com/downloads/en/DeviceDoc/2101.pdf

#ifndef EMC2101_I2C_H
#define EMC2101_I2C_H

#include "i2c_io.h"

// EMC2101 I2C device address
#define EMC2101_I2C_ADDR 0x4C

// EMC2101 registers
#define EMC2101_REG_ID              0xFD    // Product ID register
#define EMC2101_REG_INT_TEMP        0x00    // Internal Temperature register
#define EMC2101_REG_EXT_TEMP_MSB    0x01    // External temperature high byte register
#define EMC2101_REG_STATUS          0x02    // Status register
#define EMC2101_REG_CONFIG          0x03    // Configuration register
#define EMC2101_REG_DATA_RATE       0x04    // Data Rate config
#define EMC2101_REG_EXT_TEMP_LSB    0x10    // External Temperature low byte register
#define EMC2101_REG_EXT_TEMP_FORCE  0x0C    // External Temperature force setting for LUT testing

#define EMC2101_TACH_LSB            0x46    // Tach RPM data low byte
#define EMC2101_TACH_MSB            0x47    // Tach RPM data high byte
#define EMC2101_TACH_LIMIT_LSB      0x48    // Tach low-speed setting low byte. INVERSE OF THE SPEED
#define EMC2101_TACH_LIMIT_MSB      0x49    // Tach low-speed setting high byte. INVERSE OF THE SPEED
#define EMC2101_FAN_CONFIG          0x4A    // General fan config register
#define EMC2101_FAN_SPINUP          0x4B    // Fan spinup behavior settings
#define EMC2101_REG_FAN_SETTING     0x4C    // Fan speed for non-LUT settings, as a % PWM duty cycle
#define EMC2101_PWM_FREQ            0x4D    // PWM frequency setting
#define EMC2101_PWM_DIV             0x4E    // PWM frequency divisor
#define EMC2101_LUT_HYSTERESIS      0x4F    // The hysteresis value for LUT lookups when temp is decreasing

#define EMC2101_LUT_START           0x50    // The first temp threshold register

#define EMC2101_TEMP_FILTER         0xBF    // The external temperature sensor filtering behavior
#define EMC2101_REG_PARTID          0xFD    // 0x16
#define EMC2101_REG_MFGID           0xFE    // 0xFF16

#define MAX_LUT_SPEED               0x3F    // 6-bit value
#define MAX_LUT_TEMP                0x7F    // 7-bit

// EMC2101 Product ID definitions
#define EMC2101_ID_DEFAULT          0x16    // EMC2101 default device id from part id
#define EMC2101_ID_ALTERNATE        0x28    // EMC2101 alternate device id from part id

// EMC2101 Data Rate definitions
#define EMC2101_RATE_1_16_HZ        0x00    // 1_16_HZ
#define EMC2101_RATE_1_8_HZ         0x01    // 1_8_HZ
#define EMC2101_RATE_1_4_HZ         0x02    // 1_4_HZ
#define EMC2101_RATE_1_2_HZ         0x03    // 1_2_HZ
#define EMC2101_RATE_1_HZ           0x04    // 1_HZ
#define EMC2101_RATE_2_HZ           0x05    // 2_HZ
#define EMC2101_RATE_4_HZ           0x06    // 4_HZ
#define EMC2101_RATE_8_HZ           0x07    // 8_HZ
#define EMC2101_RATE_16_HZ          0x08    // 16_HZ
#define EMC2101_RATE_32_HZ          0x09    // 32_HZ

// EMC2101 External Temperature Low Byte definitions
#define EMC2101_EXT_TEMP_MSB_DIODE_FAULT_CHECK  0x7f
#define EMC2101_EXT_TEMP_LSB_DIODE_FAULT_OPEN   0x00
#define EMC2101_EXT_TEMP_LSB_DIODE_FAULT_SHORT  0xe0
#define EMC2101_DIODE_FAULT_OPEN    1016.f
#define EMC2101_DIODE_FAULT_SHORT   1023.f

// Other definitions
#define EMC2101_FAN_RPM_NUMERATOR   5400000 // Conversion unit to convert LSBs to fan RPM

// EMC2101 error codes
#define EMC2101_SUCCESS                 I2C_SUCCESS
#define EMC2101_INVALID_ID              I2C_USER_ERROR(-1)
#define EMC2101_WRITE_DATA_RATE_ERROR   I2C_USER_ERROR(-2)
#define EMC2101_INVALID_DATA_RATE       I2C_USER_ERROR(-3)

#endif // EMC2101_I2C_H
