// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// I2C I/O with I2C device file descriptor

#ifndef I2C_DEV_IO_H
#define I2C_DEV_IO_H

#include "i2c_dev_context.h"

int smbus_read_reg_u8(context* context, int reg, uint8_t* value);
int smbus_read_reg_u16(context* context, int reg, uint16_t* value);
int smbus_read_reg_u16_be(context* context, int reg, uint16_t* value);

int smbus_write_reg_u8(context* context, int reg, uint8_t value);
int smbus_write_reg_u16(context* context, int reg, uint16_t value);
int smbus_write_reg_u16_be(context* context, int reg, uint16_t value);

#endif // I2C_DEV_IO_H
