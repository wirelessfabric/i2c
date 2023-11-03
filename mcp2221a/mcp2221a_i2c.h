// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// MCP2221A - Microchip USB to I2C Host Protocol Bridge
// Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/20005565B.pdf

#ifndef MCP2221A_I2C_H
#define MCP2221A_I2C_H

#include "mcp2221a_hid.h"
#include "i2c_io.h"

// I2C command code field
#define I2C_COMMAND                     MCP2221A_COMMAND
#define I2C_SET_COMMAND                 0x10
#define I2C_GET_STATUS_COMMAND          I2C_SET_COMMAND
#define I2C_GET_DATA_COMMAND            0x40
#define I2C_WRITE_COMMAND               0x90
#define I2C_READ_COMMAND                0x91
#define I2C_READ_REPEATED_START_COMMAND 0x93
#define I2C_WRITE_NO_STOP_COMMAND       0x94

// I2C set command fields
#define I2C_SET_COMMAND_DONT_CARE       2
#define I2C_SET_COMMAND_I2C_CANCEL      3
#define I2C_SET_COMMAND_I2C_SPEED       4
#define I2C_SET_COMMAND_I2C_DIVISOR     5

// I2C read and write command fields
#define I2C_RW_COMMAND_SIZE_LSB         2
#define I2C_RW_COMMAND_SIZE_MSB         3
#define I2C_RW_COMMAND_I2C_ADDR         4
#define I2C_RW_COMMAND_DATA             5
#define I2C_RW_DATA_SIZE                (MCP2221A_WRITE_SIZE - I2C_RW_COMMAND_DATA)

// I2C response common fields
#define I2C_RESPONSE                    MCP2221A_RESPONSE
#define I2C_COMMAND_COMPLETION          1

// I2C status response fields
#define I2C_STATUS_RESPONSE_I2C_CANCEL  2
#define I2C_STATUS_RESPONSE_I2C_SPEED   3
#define I2C_STATUS_RESPONSE_I2C_DIVISOR 4
#define I2C_STATUS_RESPONSE_I2C_STATE   8

// I2C read and write response fields
#define I2C_RW_RESPONSE_I2C_STATE       2
#define I2C_RW_RESPONSE_SIZE            3
#define I2C_RW_RESPONSE_DATA            4

// internal I2C state machine states
#define I2C_IDLE                        0x00
#define I2C_BUSY                        0x01
#define I2C_CANCEL                      0x10
#define I2C_IDLE_ALREADY                0x11
#define I2C_START_TIMEOUT               0x12
#define I2C_ADDR_NACK                   0x25
#define I2C_PARTIAL_DATA                0x41
#define I2C_WRITE_NO_STOP               0x45
#define I2C_READ_COMPLETION             0x55
#define I2C_STOP_TIMEOUT                0x60

// I2C speed codes
#define I2C_SET_SPEED                   0x20
#define I2C_SPEED_NOT_SET               0x21
#define I2C_DIVISOR_KHZ(frequency)      ((12000000 / ((frequency) * 1000)) - 3)
#define I2C_DIVISOR_400KHZ              I2C_DIVISOR_KHZ(400)
#define I2C_DIVISOR_100KHZ              I2C_DIVISOR_KHZ(100)

// I2c addressing
#define I2C_WRITE_ADDR(addr)           (uint8_t)(((addr) << 1) & 0xFF)
#define I2C_READ_ADDR(addr)            (uint8_t)((((addr) << 1) + 1) & 0xFF)

int mcp2221a_i2c_command(context* context, int command, uint8_t* buffer);
int mcp2221a_i2c_set_speed(context* context);
int mcp2221a_i2c_cancel(context* context);
int mcp2221a_i2c_get_state(context* context, uint8_t* i2c_state);
void mcp2221a_i2c_show_status(context* context, size_t index, size_t size);
int mcp2221a_i2c_configure(context* context);

int mcp2221a_i2c_transfer_data(context* context, uint8_t command, uint8_t* buffer, size_t size);
int mcp2221a_i2c_write(context* context, uint8_t* buffer, size_t size);
int mcp2221a_i2c_read(context* context, uint8_t* buffer, size_t write, size_t size);

int mcp2221a_i2c_scan_one(context* context, int i2c_addr);
void mcp2221a_i2c_scan(context* context);

#endif // MCP2221A_I2C_H
