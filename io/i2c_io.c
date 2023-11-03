// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// I2C I/O common functions

#include "i2c_io.h"

///////////////////////////////////////////////////////////////////////////////

int i2c_read_module_u32_be(context* context, int base, int reg, uint32_t* value) {
    uint8_t big_endian[4];
    int result = i2c_read_module(context, base, reg, big_endian, sizeof(big_endian));
    if (result < 0)
        return result;

    if (value) {
        uint8_t* little_endian = (uint8_t*)value;
        little_endian[0] = big_endian[3];
        little_endian[1] = big_endian[2];
        little_endian[2] = big_endian[1];
        little_endian[3] = big_endian[0];
    }

    return I2C_SUCCESS;
}

int i2c_write_module_u32_be(context* context, int base, int reg, uint32_t value) {
    uint8_t* little_endian = (uint8_t*)&value;
    uint8_t command[6];
    command[0] = (uint8_t)base;
    command[1] = (uint8_t)reg;
    command[2] = little_endian[3];
    command[3] = little_endian[2];
    command[4] = little_endian[1];
    command[5] = little_endian[0];
    return i2c_write(context, command, sizeof(command));
}

int i2c_read_module_s32_be(context* context, int base, int reg, int32_t* value) {
    return i2c_read_module_u32_be(context, base, reg, (uint32_t*)value);
}

int i2c_write_module_s32_be(context* context, int base, int reg, int32_t value) {
    return i2c_write_module_u32_be(context, base, reg, (uint32_t)value);
}

int i2c_read_module_u16_be(context* context, int base, int reg, uint16_t* value) {
    uint8_t big_endian[2];
    int result = i2c_read_module(context, base, reg, big_endian, sizeof(big_endian));
    if (result < 0)
        return result;

    if (value) {
        uint8_t* little_endian = (uint8_t*)value;
        little_endian[0] = big_endian[1];
        little_endian[1] = big_endian[0];
    }

    return I2C_SUCCESS;
}

int i2c_read_module_u8(context* context, int base, int reg, uint8_t* byte) {
    return i2c_read_module(context, base, reg, byte, sizeof(*byte));
}

int i2c_write_module_u8(context* context, int base, int reg, uint8_t byte) {
    uint8_t command[3];
    command[0] = (uint8_t)base;
    command[1] = (uint8_t)reg;
    command[2] = byte;
    return i2c_write(context, command, sizeof(command));
}

///////////////////////////////////////////////////////////////////////////////

int i2c_read_reg_u24_be(context* context, int reg, uint32_t* value) {
    uint8_t big_endian[3];
    int result = i2c_read_reg(context, reg, big_endian, sizeof(big_endian));
    if (result < 0)
        return result;

    if (value) {
        uint8_t* little_endian = (uint8_t*)value;
        little_endian[0] = big_endian[2];
        little_endian[1] = big_endian[1];
        little_endian[2] = big_endian[0];
        little_endian[3] = 0;
    }

    return I2C_SUCCESS;
}

int i2c_read_reg_u16_be(context* context, int reg, uint16_t* value) {
    uint8_t big_endian[2];
    int result = i2c_read_reg(context, reg, big_endian, sizeof(big_endian));
    if (result < 0)
        return result;

    if (value) {
        uint8_t* little_endian = (uint8_t*)value;
        little_endian[0] = big_endian[1];
        little_endian[1] = big_endian[0];
    }

    return I2C_SUCCESS;
}

int i2c_read_reg_u16_le(context* context, int reg, uint16_t* value) {
    return i2c_read_reg(context, reg, value, sizeof(*value));
}

int i2c_write_reg_u8(context* context, int reg, uint8_t byte) {
    uint8_t command[2];
    command[0] = (uint8_t)reg;
    command[1] = byte;
    return i2c_write(context, command, sizeof(command));
}

int i2c_read_reg_u8(context* context, int reg, uint8_t* byte) {
    return i2c_read_reg(context, reg, byte, sizeof(*byte));
}

///////////////////////////////////////////////////////////////////////////////

uint32_t i2c_read_u24_be(context* context, int reg) {
    uint32_t value;
    return i2c_read_reg_u24_be(context, reg, &value) == 0 ? value : 0;
}

uint16_t i2c_read_u16_be(context* context, int reg) {
    uint16_t value;
    return i2c_read_reg_u16_be(context, reg, &value) == 0 ? value : 0;
}

uint16_t i2c_read_u16_le(context* context, int reg) {
    uint16_t value;
    return i2c_read_reg_u16_le(context, reg, &value) == 0 ? value : 0;
}

int16_t i2c_read_s16_le(context* context, int reg) {
    return (int16_t)i2c_read_u16_le(context, reg);
}

uint8_t i2c_read_u8(context* context, int reg) {
    uint8_t byte;
    return i2c_read_reg_u8(context, reg, &byte) == 0 ? byte : 0;
}

int8_t i2c_read_s8(context* context, int reg) {
    return (int8_t)i2c_read_u8(context, reg);
}

///////////////////////////////////////////////////////////////////////////////

void i2c_set_debug(context* context, bool debug) {
    context->i2c.debug = debug;
}

void i2c_set_dump(context* context, bool dump) {
    context->i2c.dump = dump;
}

void i2c_set_addr(context* context, int addr) {
    context->i2c.addr = addr;
}

void i2c_set_delay_usec(context* context, unsigned int usec) {
    context->i2c.delay_usec = usec;
}

void i2c_dump_buffer(context* context, const char* text, uint8_t* buffer) {
    (void)context;
    printf("%s " \
           "%02x %02x %02x %02x %02x %02x %02x %02x " \
           "%02x %02x %02x %02x %02x %02x %02x %02x " \
           "%02x %02x %02x %02x %02x %02x %02x %02x " \
           "%02x %02x %02x %02x %02x %02x %02x %02x\n",
           text,
           buffer[0],  buffer[1],  buffer[2],  buffer[3],
           buffer[4],  buffer[5],  buffer[6],  buffer[7],
           buffer[8],  buffer[9],  buffer[10], buffer[11],
           buffer[12], buffer[13], buffer[14], buffer[15],
           buffer[16], buffer[17], buffer[18], buffer[19],
           buffer[20], buffer[21], buffer[22], buffer[23],
           buffer[24], buffer[25], buffer[26], buffer[27],
           buffer[28], buffer[29], buffer[30], buffer[31]);
}
