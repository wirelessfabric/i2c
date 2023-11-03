// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// I2C device context

#ifndef I2C_DEV_CONTEXT_H
#define I2C_DEV_CONTEXT_H

#include <stdbool.h>

#ifndef I2C_BUS
#define I2C_BUS "/dev/i2c-1" /* Raspberry Pi 4 default I2C bus */
#endif

typedef struct {
    int addr;
    const char* bus;
    int dev; // device file descriptor
    unsigned int delay_usec;
    bool debug;
    bool dump;
} i2c_context;

typedef struct {
    i2c_context i2c;
} context;

#endif // I2C_DEV_CONTEXT_H
