// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// I2C I/O with USB HID device handle

#include <stdio.h>
#include <stdlib.h> // strtol, strtoul
#include "mcp2221a_i2c.h"

#include "i2c_io.c" // common functions

int i2c_open(context* context, int argc, char *argv[]) {
    int index = 1;
    while (index < argc) {
        const char* arg = argv[index++];
        if (strcmp(arg, "--dbg") == 0)
            i2c_set_debug(context, true);
        else if (strcmp(arg, "--dump") == 0)
            i2c_set_dump(context, true);
        else if (strncmp(arg, "--i2c_addr=", 11) == 0)
            i2c_set_addr(context, strtol(&arg[11], NULL, 0));
        else if (strncmp(arg, "--i2c_delay=", 12) == 0)
            i2c_set_delay_usec(context, strtoul(&arg[12], NULL, 0));
    }

    if (mcp2221a_hid_init(context, argc, argv) < 0)
        return I2C_OPEN_INIT_ERROR;

    if (mcp2221a_hid_open(context) < 0) {
        mcp2221a_hid_exit(context);
        return I2C_OPEN_ERROR;
    }

    if (i2c_debug(context))
        mcp2221a_hid_show_info(context);

    int result = mcp2221a_i2c_configure(context);
    if (result < 0) {
        i2c_close(context);
        return I2C_CONFIG_ERROR;
    }

    if (i2c_debug(context))
        mcp2221a_i2c_show_status(context, 0, MCP2221A_READ_SIZE);

    return I2C_SUCCESS;
}

void i2c_close(context* context) {
    mcp2221a_hid_exit(context);
}

///////////////////////////////////////////////////////////////////////////////

int i2c_read(context* context, void* data, size_t size) {
    uint8_t buffer[I2C_HID_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    if (size > I2C_RW_DATA_SIZE)
        size = I2C_RW_DATA_SIZE;

    int result = mcp2221a_i2c_read(context, buffer, 0, size);
    if (result == I2C_SUCCESS && data) {
        size_t response_size = buffer[I2C_RW_RESPONSE_SIZE];
        if (response_size < size)
            size = response_size;
        memcpy(data, &buffer[I2C_RW_RESPONSE_DATA], size);
    }

    return result;
}

int i2c_write(context* context, void* data, size_t size) {
    uint8_t buffer[I2C_HID_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    if (data && size < I2C_RW_DATA_SIZE)
        memcpy(&buffer[I2C_RW_COMMAND_DATA], data, size);
    else
        size = 0;

    return mcp2221a_i2c_write(context, buffer, size);
}

///////////////////////////////////////////////////////////////////////////////

int i2c_read_module(context* context, int base, int reg, void* data, size_t size) {
    if (!data || !size) {
        if (i2c_debug(context))
            printf("i2c_read_module(0x%02x,0x%02x) invalid parameters\n",
                   base, reg);
        return I2C_DATA_PARAMETER_ERROR;
    }

    uint8_t buffer[I2C_HID_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    uint8_t* command = &buffer[I2C_RW_COMMAND_DATA];
    size_t sizeof_command = 2 * sizeof(uint8_t);
    command[0] = (uint8_t)base;
    command[1] = (uint8_t)reg;

    int result = mcp2221a_i2c_read(context, buffer, sizeof_command, size);
    if (result == I2C_SUCCESS && data) {
        size_t response_size = buffer[I2C_RW_RESPONSE_SIZE];
        if (response_size < size)
            size = response_size;
        memcpy(data, &buffer[I2C_RW_RESPONSE_DATA], size);
    }

    return I2C_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

int i2c_read_reg(context* context, int reg, void* data, size_t size) {
    if (!data || !size) {
        if (i2c_debug(context))
            printf("i2c_read_reg(0x%02x) invalid parameters\n", reg);
        return I2C_DATA_PARAMETER_ERROR;
    }

    uint8_t buffer[I2C_HID_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    uint8_t* command = &buffer[I2C_RW_COMMAND_DATA];
    size_t sizeof_command = sizeof(uint8_t);
    command[0] = (uint8_t)reg;

    int result = mcp2221a_i2c_read(context, buffer, sizeof_command, size);
    if (result == I2C_SUCCESS && data) {
        size_t response_size = buffer[I2C_RW_RESPONSE_SIZE];
        if (response_size < size)
            size = response_size;
        memcpy(data, &buffer[I2C_RW_RESPONSE_DATA], size);
    }

    return I2C_SUCCESS;
}
