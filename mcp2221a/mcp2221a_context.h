// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// I2C USB HID I/O context

#ifndef MCP2221A_CONTEXT_H
#define MCP2221A_CONTEXT_H

#include <stddef.h>  // wchar_t
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef CONFIG_I2C_HID_IO
#define CONFIG_I2C_HID_IO
#endif

#define MAX_STR 255
#define I2C_HID_BUFFER_SIZE 65

typedef struct {
    int addr;
    unsigned int delay_usec;
    uint8_t status[I2C_HID_BUFFER_SIZE];
    bool debug;
    bool dump;
} i2c_context;

typedef struct {
    void* dev; // device handle
    const char* serial_number;
    unsigned int delay_usec;
    unsigned int reopen_delay_usec;
    wchar_t wstr[MAX_STR];
    bool debug;
} hid_context;

typedef struct {
    i2c_context i2c;
    hid_context hid;
} context;

#endif // MCP2221A_CONTEXT_H
