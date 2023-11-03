// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// MCP2221A - Microchip USB to I2C Host Protocol Bridge
// Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/20005565B.pdf

#ifndef MCP2221A_HID_H
#define MCP2221A_HID_H

#include "mcp2221a_context.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#define __USE_MISC 1 // usleep
#include <unistd.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <hidapi_winapi.h>
#elif defined(__APPLE__)
#include <hidapi_darwin.h>
#else
#include <hidapi/hidapi.h>
#endif

#define MCP2221A_USB_VID            0x4D8
#define MCP2221A_USB_PID            0xDD

#define HID_REPORT_ID               0
#define MCP2221A_HID_REPORT_ID      0x00

// command code field
#define MCP2221A_COMMAND            1
#define MCP2221A_RESET_CHIP_COMMAND 0x70

// response code field
#define MCP2221A_RESPONSE           0

// transfer definitions
#define MCP2221A_WRITE_SIZE         I2C_HID_BUFFER_SIZE
#define MCP2221A_READ_SIZE          (MCP2221A_WRITE_SIZE - 1)

// error codes
#define MCP2221A_SUCCESS            0
#define MCP2221A_OPEN_ERROR         -1
#define MCP2221A_RESET_ERROR        -2
#define MCP2221A_REOPEN_ERROR       -3
#define MCP2221A_WRITE_ERROR        -4
#define MCP2221A_READ_ERROR         -5

int mcp2221a_hid_init(context* context, int argc, char* argv[]);
void mcp2221a_hid_exit(context* context);

int mcp2221a_hid_open(context* context);
void mcp2221a_hid_close(context* context);

int mcp2221a_hid_write(context* context, uint8_t* buffer);
int mcp2221a_hid_read(context* context, uint8_t* buffer);
int mcp2221a_hid_transfer(context* context, uint8_t* buffer);

int mcp2221a_hid_reset(context* context);

void mcp2221a_hid_show_info(context* context);

#endif // MCP2221A_HID_H
