// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// I2C I/O common header

#ifndef I2C_IO_H
#define I2C_IO_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#define __USE_MISC 1 // usleep
#include <unistd.h>
#endif

#ifdef CONFIG_I2C_HID_IO
#include "i2c_hid_io.h"
#else
#include "i2c_dev_io.h"
#endif

#ifndef MIN_I2C_ADDR
#define MIN_I2C_ADDR 0x08    /* Raspberry Pi 4 minimum I2C address */
#endif

#ifndef MAX_I2C_ADDR
#define MAX_I2C_ADDR 0x77    /* Raspberry Pi 4 maximum I2C address */
#endif

// error codes
#define I2C_SUCCESS                     0
#define I2C_ADDRESS_ERROR               -1
#define I2C_OPEN_INIT_ERROR             -2
#define I2C_OPEN_ERROR                  -3
#define I2C_IOCTL_ERROR                 -4
#define I2C_COMMAND_TRANSFER_ERROR      -5
#define I2C_COMMAND_ECHO_ERROR          -6
#define I2C_COMMAND_COMPLETION_ERROR    -7
#define I2C_SET_SPEED_NOT_SET_ERROR     -8
#define I2C_SET_SPEED_ECHO_ERROR        -9
#define I2C_SET_SPEED_DIVISOR_ERROR     -10
#define I2C_DATA_PARAMETER_ERROR        -11
#define I2C_DATA_TRANSFER_POLL_ERROR    -12
#define I2C_DATA_TRANSFER_ERROR         -13
#define I2C_READ_ERROR                  -14
#define I2C_READ_SIZE_ERROR             -15
#define I2C_WRITE_ERROR                 -16
#define I2C_WRITE_SIZE_ERROR            -17
#define I2C_SCAN_CANCEL_ERROR           -18
#define I2C_CONFIG_ERROR                -19
#define I2C_GET_STATE_ERROR             -20
#define I2C_CANCEL_ERROR                -21
#define I2C_SMBUS_SIZE_ERROR            -22
#define I2C_SMBUS_TRANSFER_ERROR        -23
#define I2C_MAX_ERROR                   -39
#define I2C_USER_ERROR(code)            (I2C_MAX_ERROR + (code))

///////////////////////////////////////////////////////////////////////////////

int i2c_open(context* context, int argc, char *argv[]);
void i2c_close(context* context);

int i2c_read(context* context, void* data, size_t size);
int i2c_write(context* context, void* data, size_t size);

///////////////////////////////////////////////////////////////////////////////

int i2c_read_module(context* context, int base, int reg, void* buffer, size_t size);
int i2c_read_module_u32_be(context* context, int base, int reg, uint32_t* value);
int i2c_write_module_u32_be(context* context, int base, int reg, uint32_t value);
int i2c_read_module_s32_be(context* context, int base, int reg, int32_t* value);
int i2c_write_module_s32_be(context* context, int base, int reg, int32_t value);
int i2c_read_module_u16_be(context* context, int base, int reg, uint16_t* value);
int i2c_read_module_u8(context* context, int base, int reg, uint8_t* byte);
int i2c_write_module_u8(context* context, int base, int reg, uint8_t byte);

///////////////////////////////////////////////////////////////////////////////

int i2c_write_reg_u8(context* context, int reg, uint8_t byte);

int i2c_read_reg(context* context, int reg, void* data, size_t size);
int i2c_read_reg_u24_be(context* context, int reg, uint32_t* value);
int i2c_read_reg_u16_be(context* context, int reg, uint16_t* value);
int i2c_read_reg_u16_le(context* context, int reg, uint16_t* value);
int i2c_read_reg_u8(context* context, int reg, uint8_t* byte);

///////////////////////////////////////////////////////////////////////////////

uint32_t i2c_read_u24_be(context* context, int reg);
uint16_t i2c_read_u16_be(context* context, int reg);
uint16_t i2c_read_u16_le(context* context, int reg);
int16_t i2c_read_s16_le(context* context, int reg);
uint8_t i2c_read_u8(context* context, int reg);
int8_t i2c_read_s8(context* context, int reg);

///////////////////////////////////////////////////////////////////////////////

static inline void i2c_sleep(unsigned int usec) {
#if defined(_WIN32) || defined(_WIN64)
    Sleep((DWORD)(usec / 1000));
#else
    usleep(usec);
#endif
}

static inline bool i2c_debug(context* context) {
    return context->i2c.debug;
}

static inline bool i2c_dump(context* context) {
    return context->i2c.dump;
}

static inline int i2c_addr(context* context) {
    return context->i2c.addr;
}

static inline unsigned int i2c_delay_usec(context* context) {
    return context->i2c.delay_usec;
}

static inline void i2c_transfer_delay(context* context) {
    if (context->i2c.delay_usec)
        i2c_sleep(context->i2c.delay_usec);
}

void i2c_set_debug(context* context, bool debug);
void i2c_set_dump(context* context, bool dump);
void i2c_set_addr(context* context, int addr);
void i2c_set_delay_usec(context* context, unsigned int usec);

void i2c_dump_buffer(context* context, const char* text, uint8_t* buffer, size_t size);

#endif // I2C_IO_H
