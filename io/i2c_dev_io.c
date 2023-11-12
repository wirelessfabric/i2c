// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// I2C I/O with I2C device file descriptor

#include <stdio.h>
#include <stdlib.h> // strtol, strtoul
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "i2c_io.c" // common functions

int i2c_open(context* context, int argc, char *argv[]) {
    int index = 1;
    while (index < argc) {
        const char* arg = argv[index++];
        if (strcmp(arg, "--dbg") == 0)
            i2c_set_debug(context, true);
        else if (strcmp(arg, "--dump") == 0)
            i2c_set_dump(context, true);
        else if (strncmp(arg, "--i2c_bus=", 10) == 0)
            context->i2c.bus = &arg[10];
        else if (strncmp(arg, "--i2c_addr=", 11) == 0)
            i2c_set_addr(context, strtol(&arg[11], NULL, 0));
        else if (strncmp(arg, "--i2c_delay=", 12) == 0)
            i2c_set_delay_usec(context, strtoul(&arg[12], NULL, 0));
    }

    if (context->i2c.bus == NULL)
        context->i2c.bus = I2C_BUS;

    if (i2c_addr(context) < MIN_I2C_ADDR || i2c_addr(context) > MAX_I2C_ADDR) {
        if (i2c_debug(context)) {
            printf("I2C device address 0x%02x\n", i2c_addr(context));
            printf("Valid range [0x%02x,0x%02x]\n", MIN_I2C_ADDR, MAX_I2C_ADDR);
        }
        return I2C_ADDRESS_ERROR;
    }

    int file = open(context->i2c.bus, O_RDWR);
    if (file <= 0) {
        if (i2c_debug(context))
            printf("open(%s) returned %d\n", context->i2c.bus, file);
        return I2C_OPEN_ERROR;
    }

    if (ioctl(file, I2C_SLAVE, i2c_addr(context)) < 0) {
        if (i2c_debug(context))
            printf("ioctl(0x%x) set device address\n", i2c_addr(context));
        close(file);
        return I2C_IOCTL_ERROR;
    }

    context->i2c.dev = file;
    return I2C_SUCCESS;
}

void i2c_close(context* context) {
    if (context->i2c.dev > 0) {
        close(context->i2c.dev);
        context->i2c.dev = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////

int i2c_read(context* context, void* data, size_t size) {
    if (!data || !size) {
        if (i2c_debug(context))
            printf("i2c_read() invalid parameters\n");
        return I2C_DATA_PARAMETER_ERROR;
    }

    size_t result = read(context->i2c.dev, data, size);
    if (result == size)
        return I2C_SUCCESS;

    if (i2c_debug(context))
        printf("i2c_read() result %d\n", (int)result);
    return I2C_READ_SIZE_ERROR;
}

int i2c_write(context* context, void* data, size_t size) {
    if (!data || !size) {
        if (i2c_debug(context))
            printf("i2c_write() invalid parameters\n");
        return I2C_DATA_PARAMETER_ERROR;
    }

    size_t result = write(context->i2c.dev, data, size);
    if (result == size)
        return I2C_SUCCESS;

    if (i2c_debug(context))
        printf("i2c_write() result %d\n", (int)result);
    return I2C_WRITE_SIZE_ERROR;
}

///////////////////////////////////////////////////////////////////////////////

int i2c_read_module(context* context, int base, int reg, void* data, size_t size) {
    if (!data || !size) {
        if (i2c_debug(context))
            printf("i2c_read_module(0x%02x,0x%02x) invalid parameters\n",
                   base, reg);
        return I2C_DATA_PARAMETER_ERROR;
    }

    uint8_t command[2];
    command[0] = (uint8_t)base;
    command[1] = (uint8_t)reg;
    size_t result = write(context->i2c.dev, command, sizeof(command));
    if (result != sizeof(command)) {
        if (i2c_debug(context))
            printf("i2c_read_module(0x%02x,0x%02x) write result %d\n",
                   base, reg, (int)result);
        return I2C_WRITE_ERROR;
    }

    i2c_transfer_delay(context);

    result = read(context->i2c.dev, data, size);
    if (result == size)
        return I2C_SUCCESS;

    if (i2c_debug(context))
        printf("i2c_read_module(0x%02x,0x%02x) read result %d\n",
               base, reg, (int)result);
    return I2C_READ_ERROR;
}

///////////////////////////////////////////////////////////////////////////////

int i2c_read_reg(context* context, int reg, void* data, size_t size) {
    if (!data || !size) {
        if (i2c_debug(context))
            printf("i2c_read_reg(0x%02x) invalid parameters\n", reg);
        return I2C_DATA_PARAMETER_ERROR;
    }

    uint8_t command = (uint8_t)reg;
    size_t result = write(context->i2c.dev, &command, sizeof(command));
    if (result != sizeof(command)) {
        if (i2c_debug(context))
            printf("i2c_read_reg(0x%02x) write result %d\n", reg, (int)result);
        return I2C_WRITE_ERROR;
    }

    i2c_transfer_delay(context);

    result = read(context->i2c.dev, data, size);
    if (result == size)
        return I2C_SUCCESS;

    if (i2c_debug(context))
        printf("i2c_read_reg(0x%02x) read result %d\n", reg, (int)result);
    return I2C_READ_ERROR;
}

///////////////////////////////////////////////////////////////////////////////

int smbus_transfer_reg(context* context, int reg, struct i2c_smbus_ioctl_data* args, size_t size) {
    args->command = reg;

    if (size == sizeof(uint8_t))
        args->size = I2C_SMBUS_BYTE_DATA;
    else if (size == sizeof(uint16_t))
        args->size = I2C_SMBUS_WORD_DATA;
    else {
        if (i2c_debug(context))
            printf("smbus_read_reg(0x%02x) size error\n", reg);
        return I2C_SMBUS_SIZE_ERROR;
    }

    if (args->read_write == I2C_SMBUS_WRITE && i2c_dump(context))
        i2c_dump_buffer(context, "> ", (uint8_t*)args->data, args->size);

    int result = ioctl(context->i2c.dev, I2C_SMBUS, args);

    if (args->read_write == I2C_SMBUS_READ && i2c_dump(context))
        i2c_dump_buffer(context, "< ", (uint8_t*)args->data, args->size);

    if (result < 0) {
        if (i2c_debug(context))
            printf("smbus_write_reg(0x%02x) ioctl result %d\n", reg, result);
        return I2C_SMBUS_TRANSFER_ERROR;
    }

    return I2C_SUCCESS;
}

int smbus_read_reg(context* context, int reg, union i2c_smbus_data* data, size_t size) {
    struct i2c_smbus_ioctl_data args;
    args.read_write = I2C_SMBUS_READ;
    args.data = data;
    return smbus_transfer_reg(context, reg, &args, size);
}

int smbus_write_reg(context* context, int reg, union i2c_smbus_data* data, size_t size) {
    struct i2c_smbus_ioctl_data args;
    args.read_write = I2C_SMBUS_WRITE;
    args.data = data;
    return smbus_transfer_reg(context, reg, &args, size);
}

int smbus_read_reg_u8(context* context, int reg, uint8_t* value) {
    union i2c_smbus_data data;
    int result = smbus_read_reg(context, reg, &data, sizeof(uint8_t));
    if (result < 0)
        return result;

    if (value)
        *value = data.byte;

    return I2C_SUCCESS;
}

int smbus_read_reg_u16(context* context, int reg, uint16_t* value) {
    union i2c_smbus_data data;
    int result = smbus_read_reg(context, reg, &data, sizeof(uint16_t));
    if (result < 0)
        return 0;

    if (value)
        *value = data.word;

    return I2C_SUCCESS;
}

int smbus_read_reg_u16_be(context* context, int reg, uint16_t* value) {
    union i2c_smbus_data data;
    int result = smbus_read_reg(context, reg, &data, sizeof(uint16_t));
    if (result < 0)
        return result;

    if (value)
        *value = (data.block[1] << 8) | data.block[0];

    return I2C_SUCCESS;
}

int smbus_write_reg_u8(context* context, int reg, uint8_t value) {
    union i2c_smbus_data data;
    data.byte = value;
    return smbus_write_reg(context, reg, &data, sizeof(uint8_t));
}

int smbus_write_reg_u16(context* context, int reg, uint16_t value) {
    union i2c_smbus_data data;
    data.word = value;
    return smbus_write_reg(context, reg, &data, sizeof(uint16_t));
}

int smbus_write_reg_u16_be(context* context, int reg, uint16_t value) {
    union i2c_smbus_data data;
    data.block[0] = (value >> 8) & 0xFF;
    data.block[1] = value & 0xFF;
    return smbus_write_reg(context, reg, &data, sizeof(uint16_t));
}
