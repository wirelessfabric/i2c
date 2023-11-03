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

#include <stdio.h>

#include "mcp2221a_i2c.h"

#define I2C_DATA_TRANSFER_POLLS 50

int mcp2221a_i2c_command(context* context, int command, uint8_t* buffer) {
    buffer[I2C_COMMAND] = command;

    if (i2c_dump(context))
        i2c_dump_buffer(context, "i2c_command >", &buffer[I2C_COMMAND]);

    int result = mcp2221a_hid_transfer(context, buffer);

    if (i2c_dump(context))
        i2c_dump_buffer(context, "i2c_command <", &buffer[I2C_RESPONSE]);

    if (result < 0) {
        if (i2c_debug(context))
            printf("i2c_command: transfer failed\n");
        return I2C_COMMAND_TRANSFER_ERROR;
    }

    uint8_t echo = buffer[I2C_RESPONSE];
    if (echo != command) {
        if (i2c_debug(context))
            printf("i2c_command: unexpected command echo 0x%02x\n", echo);
        return I2C_COMMAND_ECHO_ERROR;
    }

    uint8_t status = buffer[I2C_COMMAND_COMPLETION];
    if (status != I2C_SUCCESS) {
        if (i2c_debug(context))
            printf("i2c_command: command completion error 0x%02x\n", status);
        return I2C_COMMAND_COMPLETION_ERROR;
    }

    return I2C_SUCCESS;
}

int mcp2221a_i2c_set_speed(context* context) {
    uint8_t buffer[MCP2221A_WRITE_SIZE];
    memset(buffer, 0, sizeof(buffer));
    buffer[I2C_SET_COMMAND_I2C_SPEED] = I2C_SET_SPEED;

    uint8_t i2c_divisor = I2C_DIVISOR_400KHZ;
    buffer[I2C_SET_COMMAND_I2C_DIVISOR] = i2c_divisor;

    int result = mcp2221a_i2c_command(context, I2C_SET_COMMAND, buffer);
    if (result < 0) {
        if (i2c_debug(context))
            printf("i2c_set_speed: set failed\n");
        return result;
    }

    uint8_t echo = buffer[I2C_STATUS_RESPONSE_I2C_SPEED];
    if (echo == I2C_SPEED_NOT_SET) {
        if (i2c_debug(context))
            printf("i2c_set_speed: not set\n");
        return I2C_SET_SPEED_NOT_SET_ERROR;
    }
    else if (echo != I2C_SET_SPEED) {
        if (i2c_debug(context))
            printf("i2c_set_speed: unexpected set speed echo 0x%02x\n", echo);
        return I2C_SET_SPEED_ECHO_ERROR;
    }

    uint8_t divisor = buffer[I2C_STATUS_RESPONSE_I2C_DIVISOR];
    if (divisor != i2c_divisor) {
        if (i2c_debug(context))
            printf("i2c_set_speed: unexpected i2c divisor 0x%02x\n", divisor);
        return I2C_SET_SPEED_DIVISOR_ERROR;
    }

    return I2C_SUCCESS;
}

int mcp2221a_i2c_cancel(context* context) {
    uint8_t buffer[MCP2221A_WRITE_SIZE];
    memset(buffer, 0, sizeof(buffer));
    buffer[I2C_SET_COMMAND_I2C_CANCEL] = I2C_CANCEL;

    int result = mcp2221a_i2c_command(context, I2C_SET_COMMAND, buffer);
    if (result < 0) {
        if (i2c_debug(context))
            printf("i2c_cancel: set failed\n");
        return result;
    }

    uint8_t i2c_cancel = buffer[I2C_STATUS_RESPONSE_I2C_CANCEL];
    if (i2c_cancel == I2C_CANCEL)
        i2c_sleep(1000);

    return I2C_SUCCESS;
}

int mcp2221a_i2c_get_state(context* context, uint8_t* state) {
    uint8_t* buffer = context->i2c.status;
    memset(buffer, 0, sizeof(context->i2c.status));
    int result = mcp2221a_i2c_command(context, I2C_GET_STATUS_COMMAND, buffer);
    if (result < 0) {
        if (i2c_debug(context))
            printf("i2c_get_state: set failed\n");
        return result;
    }
    
    if (state)
        *state = buffer[I2C_STATUS_RESPONSE_I2C_STATE];

    return I2C_SUCCESS;
}

void mcp2221a_i2c_show_status(context* context, size_t index, size_t size) {
    uint8_t* buffer = context->i2c.status;
    for (; index < size; index++) {
        if (buffer[index] != 0)
            printf("[%02zu]: 0x%02X (%d)\n",
                   index, buffer[index], buffer[index]);
    }
}

int mcp2221a_i2c_configure(context* context) {
    int result = mcp2221a_i2c_set_speed(context);
    if (result < 0) {
        if (i2c_debug(context))
            printf("i2c_configure: set speed failed\n");

        result = mcp2221a_i2c_cancel(context);
        if (result == I2C_SUCCESS) {
            result = mcp2221a_i2c_set_speed(context);
            if (result < 0 && i2c_debug(context))
                printf("i2c_configure: set speed retry failed\n");
        }
        else if (i2c_debug(context))
            printf("i2c_configure: cancel failed\n");
    }
    if (result < 0)
        return result;

    uint8_t i2c_state;
    result = mcp2221a_i2c_get_state(context, &i2c_state);
    if (result < 0)
        return result;

    if (i2c_state == I2C_IDLE && i2c_debug(context))
        printf("Internal I2C state machine is idle\n");
    return I2C_SUCCESS;
}

int mcp2221a_i2c_transfer_data(context* context, uint8_t command, uint8_t* buffer, size_t size) {
    buffer[I2C_RW_COMMAND_SIZE_LSB] = (uint8_t)(size & 0xFF);
    buffer[I2C_RW_COMMAND_SIZE_MSB] = (uint8_t)((size >> 8) & 0xFF);

    int result = mcp2221a_i2c_command(context, command, buffer);
    if (result < 0)
        return result;

    uint8_t i2c_state = buffer[I2C_RW_RESPONSE_I2C_STATE];
    if (i2c_state == I2C_IDLE)
        return I2C_SUCCESS;

    for (int poll=0; poll < I2C_DATA_TRANSFER_POLLS; poll++) {
        i2c_sleep(1000);

        if (mcp2221a_i2c_get_state(context, &i2c_state) < 0)
            return I2C_DATA_TRANSFER_POLL_ERROR;

        if (i2c_state == I2C_IDLE)
            return I2C_SUCCESS;

        if (i2c_state == I2C_ADDR_NACK)
            return -I2C_ADDR_NACK;

        if (command == I2C_WRITE_NO_STOP_COMMAND && i2c_state == I2C_WRITE_NO_STOP)
            return I2C_SUCCESS;

        if (command == I2C_READ_COMMAND || command == I2C_READ_REPEATED_START_COMMAND) {
            if (i2c_state == I2C_READ_COMPLETION)
                return mcp2221a_i2c_command(context, I2C_GET_DATA_COMMAND, buffer);
        }
    }

    return I2C_DATA_TRANSFER_ERROR;
}

int mcp2221a_i2c_write(context* context, uint8_t* buffer, size_t size) {
    buffer[I2C_RW_COMMAND_I2C_ADDR] = I2C_WRITE_ADDR(i2c_addr(context));
    return mcp2221a_i2c_transfer_data(context, I2C_WRITE_COMMAND, buffer, size);
}

int mcp2221a_i2c_read(context* context, uint8_t* buffer, size_t write, size_t size) {
    buffer[I2C_RW_COMMAND_I2C_ADDR] = I2C_WRITE_ADDR(i2c_addr(context));
    int result = mcp2221a_i2c_transfer_data(context, I2C_WRITE_COMMAND, buffer, write);
    if (result < 0)
        return result;

    i2c_transfer_delay(context);

    if (size > I2C_RW_DATA_SIZE)
        size = I2C_RW_DATA_SIZE;

    memset(buffer, 0, MCP2221A_WRITE_SIZE);
    buffer[I2C_RW_COMMAND_I2C_ADDR] = I2C_READ_ADDR(i2c_addr(context));
    return mcp2221a_i2c_transfer_data(context, I2C_READ_COMMAND, buffer, size);
}

int mcp2221a_i2c_scan_one(context* context, int i2c_addr) {
    uint8_t buffer[MCP2221A_WRITE_SIZE];
    memset(buffer, 0, sizeof(buffer));
    buffer[I2C_RW_COMMAND_I2C_ADDR] = I2C_WRITE_ADDR(i2c_addr);

    int result = mcp2221a_i2c_transfer_data(context, I2C_WRITE_COMMAND, buffer, sizeof(uint8_t));
    if (result == -I2C_ADDR_NACK)
        mcp2221a_i2c_cancel(context);
    else if (result != I2C_SUCCESS)
        printf("i2c_scan_one(0x%02x): 0x%02x 0x%02x 0x%02x\n",
               i2c_addr,
               buffer[I2C_RESPONSE],
               buffer[I2C_COMMAND_COMPLETION],
               buffer[I2C_RW_RESPONSE_I2C_STATE]);

    return result;
}

void mcp2221a_i2c_scan(context* context) {
    for (int i2c_addr = MIN_I2C_ADDR; i2c_addr <= MAX_I2C_ADDR; i2c_addr++) {
        int result = mcp2221a_i2c_scan_one(context, i2c_addr);
        if (result == I2C_SUCCESS)
            printf("i2c_scan(0x%02x): ACK\n", i2c_addr);
        else if (result == -I2C_ADDR_NACK && i2c_debug(context))
            printf("i2c_scan(0x%02x): NACK\n", i2c_addr);
        i2c_sleep(1000);
    }
}
