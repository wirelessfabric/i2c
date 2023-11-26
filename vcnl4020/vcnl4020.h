// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit VCNL4020 Proximity and Light Sensor
// See: https://cdn-learn.adafruit.com/assets/assets/000/124/959/original/vcnl4020.pdf

#ifndef VCNL4020_I2C_H
#define VCNL4020_I2C_H

#include "i2c_io.h"

// VCNL4020 I2C device address
#define VCNL4020_I2C_ADDR 0x13

// VCNL4020 registers
#define VCNL4020_REG_COMMAND                    0x80
#define VCNL4020_REG_PRODUCT_ID                 0x81
#define VCNL4020_REG_PROXIMITY_RATE             0x82
#define VCNL4020_REG_IR_LED_CURRENT             0x83
#define VCNL4020_REG_AMBIENT_PARAMETERS         0x84
#define VCNL4020_REG_AMBIENT_RESULT_HIGH        0x85
#define VCNL4020_REG_AMBIENT_RESULT_LOW         0x86
#define VCNL4020_REG_PROXIMITY_RESULT_HIGH      0x87
#define VCNL4020_REG_PROXIMITY_RESULT_LOW       0x88
#define VCNL4020_REG_INT_CTRL                   0x89
#define VCNL4020_REG_LOW_THRES_HIGH             0x8A
#define VCNL4020_REG_LOW_THRES_LOW              0x8B
#define VCNL4020_REG_HIGH_THRES_HIGH            0x8C
#define VCNL4020_REG_HIGH_THRES_LOW             0x8D
#define VCNL4020_REG_INT_STATUS                 0x8E
#define VCNL4020_REG_PROX_ADJUST                0x8F

// VCNL4020 product id
#define VCNL4020_PRODUCT_ID                     0x21

// VCNL4020 command codes
#define VCNL4020_COMMAND_SELF_TIMED_ENABLE      (1 << 0) // read-write
#define VCNL4020_COMMAND_PROXIMITY_ENABLE       (1 << 1) // read-write
#define VCNL4020_COMMAND_AMBIENT_ENABLE         (1 << 2) // read-write
#define VCNL4020_COMMAND_PROXIMITY_ON_DEMAND    (1 << 3) // read-write
#define VCNL4020_COMMAND_AMBIENT_ON_DEMAND      (1 << 4) // read-write
#define VCNL4020_PROXIMITY_READY                (1 << 5) // read-only
#define VCNL4020_AMBIENT_READY                  (1 << 6) // read-only
#define VCNL4020_CONFIG_LOCK                    (1 << 7) // read-only
#define VCNL4020_ENABLE                         (VCNL4020_COMMAND_SELF_TIMED_ENABLE | \
                                                 VCNL4020_COMMAND_PROXIMITY_ENABLE | \
                                                 VCNL4020_COMMAND_AMBIENT_ENABLE)
#define VCNL4020_DISABLE                        0

// VCNL4020 proximity rate codes
#define VCNL4020_PROXMITIY_RATE_125_Hz          0x06   // 125 measurements per second
#define VCNL4020_PROXMITIY_RATE_250_Hz          0x07   // 250 measurements per second

// VCNL4020 IR LED current
#define VCNL4020_IR_LED_CURRENT_100_mA          10     // 10 decimal * 10 mA = 100 mA
#define VCNL4020_IR_LED_CURRENT_200_mA          20     // 20 decimal * 10 mA = 200 mA

// VCNL4020 ambient light averaging codes
#define VCNL4020_AMBIENT_AVERAGE_1_SAMPLE       (0x00 << 0)   //   1 sample
#define VCNL4020_AMBIENT_AVERAGE_2_SAMPLES      (0x01 << 0)   //   2 samples
#define VCNL4020_AMBIENT_AVERAGE_4_SAMPLES      (0x02 << 0)   //   4 samples
#define VCNL4020_AMBIENT_AVERAGE_8_SAMPLES      (0x03 << 0)   //   8 samples
#define VCNL4020_AMBIENT_AVERAGE_16_SAMPLES     (0x04 << 0)   //  16 samples
#define VCNL4020_AMBIENT_AVERAGE_32_SAMPLES     (0x05 << 0)   //  32 samples
#define VCNL4020_AMBIENT_AVERAGE_64_SAMPLES     (0x06 << 0)   //  64 samples
#define VCNL4020_AMBIENT_AVERAGE_128_SAMPLES    (0x07 << 0)   // 128 samples

// VCNL4020 ambient light auto offset compensation
#define VCNL4020_AMBIENT_AUTO_OFFSET_DISABLE    (0x00 << 3)
#define VCNL4020_AMBIENT_AUTO_OFFSET_ENABLE     (0x01 << 3)

// VCNL4020 ambient light rate codes
#define VCNL4020_AMBIENT_RATE_1_Hz              (0x00 << 4)   // 1 sample per second
#define VCNL4020_AMBIENT_RATE_2_Hz              (0x01 << 4)   // 2 sample per second (default)
#define VCNL4020_AMBIENT_RATE_8_Hz              (0x06 << 4)   // 8 samples per second
#define VCNL4020_AMBIENT_RATE_10_Hz             (0x07 << 4)   // 10 samples per second

// VCNL4020 ambient light continuous compensation
#define VCNL4020_AMBIENT_CONTINUOUS_DISABLE     (0x00 << 7)
#define VCNL4020_AMBIENT_CONTINUOUS_ENABLE      (0x01 << 7)

// VCNL4020 ambient light parameters initial value
#define VCNL4020_AMBIENT_PARAMETERS             (VCNL4020_AMBIENT_AVERAGE_1_SAMPLE | \
                                                 VCNL4020_AMBIENT_AUTO_OFFSET_DISABLE | \
                                                 VCNL4020_AMBIENT_RATE_10_Hz | \
                                                 VCNL4020_AMBIENT_CONTINUOUS_DISABLE)
// VCNL4020 error codes
#define VCNL4020_SUCCESS                        I2C_SUCCESS
#define VCNL4020_NO_RESPONSE                    I2C_USER_ERROR(-1)
#define VCNL4020_INVALID_PRODUCT_ID             I2C_USER_ERROR(-2)
#define VCNL4020_DISABLE_ERROR                  I2C_USER_ERROR(-3)
#define VCNL4020_SET_PROXIMITY_RATE_ERROR       I2C_USER_ERROR(-4)
#define VCNL4020_SET_IR_LED_CURRENT_ERROR       I2C_USER_ERROR(-5)
#define VCNL4020_SET_AMBIENT_PARAMETERS_ERROR   I2C_USER_ERROR(-6)
#define VCNL4020_ENABLE_ERROR                   I2C_USER_ERROR(-7)

#endif // VCNL4020_I2C_H
