// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// TLV493D-A1B6 3D Magnetic Sensor
// Low Power 3D Magnetic Sensor with I2C Interface
// User Manual:
// https://www.infineon.com/dgdl/Infineon-TLV493D-A1B6_3DMagnetic-UM-v01_03-EN.pdf?fileId=5546d46261d5e6820161e75721903ddd

#ifndef TLV493D_I2C_H
#define TLV493D_I2C_H

#include "i2c_io.h"

// TLV493D-A1B6 I2C device address
#define TLV493D_I2C_ADDR 0x5E

// TLV493D read registers
#define TLV493D_RD_BX    0
#define TLV493D_RD_BY    1
#define TLV493D_RD_BZ    2
#define TLV493D_RD_TEMP  3
#define TLV493D_RD_BX2   4
#define TLV493D_RD_BZ2   5
#define TLV493D_RD_TEMP2 6
#define TLV493D_RD_RES1  7
#define TLV493D_RD_RES2  8
#define TLV493D_RD_RES3  9
#define TLV493D_RD_REGS  10

// TLV493D write registers
#define TLV493D_WR_RES1  0
#define TLV493D_WR_MOD1  1
#define TLV493D_WR_RES2  2
#define TLV493D_WR_MOD2  3
#define TLV493D_WR_REGS  4

#define TLV493D_WR_MOD1_PARITY      0x80   // Parity bit
#define TLV493D_WR_MOD1_ADDR        0x60   // I2C address bits
#define TLV493D_WR_MOD1_FACTORY     0x18   // Factory settings
#define TLV493D_WR_MOD1_INT         0x04   // Interrupt pad enabled
#define TLV493D_WR_MOD1_FAST        0x02   // Fast mode
#define TLV493D_WR_MOD1_LOW_POWER   0x01   // Low-power mode

#define TLV493D_WR_MOD2_DISABLE     0x80   // Temperature measurement enabled
#define TLV493D_WR_MOD2_LOW_POWER   0x40   // Low-power period
#define TLV493D_WR_MOD2_PARITY      0x20   // Parity test enabled
#define TLV493D_WR_MOD2_FACTORY     0x1F   // Factory setting

#endif // TLV493D_I2C_H
