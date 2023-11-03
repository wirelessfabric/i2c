// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// MCP2221A - Microchip USB to I2C Host Protocol Bridge
// Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/20005565B.pdf
//
// http://ww1.microchip.com/downloads/en/DeviceDoc/mcp2221_0_1.tar.gz
// https://github.com/raspberrypi/linux/blob/rpi-6.1.y/drivers/hid/hid-mcp2221.c
//
// https://github.com/libusb/hidapi.git
//
// $ gcc mcp2221a_scan.c mcp2221a_hid.c mcp2221a_i2c.c -I. -I../io ../io/i2c_hid_io.c -L /usr/lib/arm-linux-gnueabihf -lhidapi-libusb -o scan_i2c_hid
// $ ./scan_i2c_hid

#include "mcp2221a_i2c.h"

static context g_context = {0};

int main(int argc, char* argv[]) {
    int result = i2c_open(&g_context, argc, argv);
    if (result < 0)
        return result;

    mcp2221a_i2c_scan(&g_context);

    i2c_close(&g_context);
    return I2C_SUCCESS;
}
