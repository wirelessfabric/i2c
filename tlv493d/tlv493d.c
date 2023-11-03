// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// TLV493D-A1B6 3D Magnetic Sensor
// Low Power 3D Magnetic Sensor with I2C Interface
// User Manual:
// https://www.infineon.com/dgdl/Infineon-TLV493D-A1B6_3DMagnetic-UM-v01_03-EN.pdf?fileId=5546d46261d5e6820161e75721903ddd
//
// $ gcc tlv493d.c ../io/i2c_dev_io.c -I../io -o tlv493d
// $ ./tlv493d

#include <stdio.h>
#include <stdlib.h>
#include "tlv493d.h"

static uint8_t read_buffer[TLV493D_RD_REGS];
static uint8_t write_buffer[TLV493D_WR_REGS];

static inline float ctof(float celsius) {
    return (celsius * 1.8f) + 32.f;
}

static int tlv493d_read(void* context) {
    size_t size = sizeof(read_buffer);
    memset(read_buffer, 0, size);
    return i2c_read(context, read_buffer, size);
}

static int tlv493d_write(void* context) {
    return i2c_write(context, write_buffer, sizeof(write_buffer));
}

static void loop(void* context) {
    while (true) {
        if (tlv493d_read(context) == 0) {
            uint8_t x1 = read_buffer[TLV493D_RD_BX];
            uint8_t y1 = read_buffer[TLV493D_RD_BY];
            uint8_t z1 = read_buffer[TLV493D_RD_BZ];
            uint8_t t1 = read_buffer[TLV493D_RD_TEMP];
            uint8_t x2 = read_buffer[TLV493D_RD_BX2];
            uint8_t z2 = read_buffer[TLV493D_RD_BZ2];
            uint8_t t2 = read_buffer[TLV493D_RD_TEMP2];

            int16_t xs = ((int16_t)x1 << 8) | ((int16_t)(x2 & 0xF0) << 0);
            int16_t ys = ((int16_t)y1 << 8) | ((int16_t)(x2 & 0x0F) << 4);
            int16_t zs = ((int16_t)z1 << 8) | ((int16_t)(z2 & 0x0F) << 4);
            int16_t ts = ((int16_t)(t1 & 0xF0) << 4) | ((int16_t)t2 << 0);

            float x = (float)(int)(xs >> 4) * 98.f;
            float y = (float)(int)(ys >> 4) * 98.f;
            float z = (float)(int)(zs >> 4) * 98.f;
            float t = (float)(int)(ts - 340) * 1.1f + 25.f;
            printf("X: %7.2f\tY: %7.2f\tZ: %7.2f\tT: %6.2f C %6.2f F\n", x, y, z, t, ctof(t));
        }
        i2c_sleep(100000);
    }
}

static int setup(void* context) {
    int result = tlv493d_read(context);
    if (result == I2C_READ_SIZE_ERROR)
        printf("ERROR: setup() no response from sensor\n");
    if (result < 0)
        return result;

    uint8_t mod1 = read_buffer[TLV493D_RD_RES1];
    mod1 &= TLV493D_WR_MOD1_FACTORY;
    mod1 |= TLV493D_WR_MOD1_PARITY;
    mod1 |= TLV493D_WR_MOD1_FAST;
    mod1 |= TLV493D_WR_MOD1_LOW_POWER;

    uint8_t res2 = read_buffer[TLV493D_RD_RES2];

    uint8_t mod2 = read_buffer[TLV493D_RD_RES3];
    mod2 &= TLV493D_WR_MOD2_FACTORY;

    write_buffer[TLV493D_WR_RES1] = 0x00;
    write_buffer[TLV493D_WR_MOD1] = mod1;
    write_buffer[TLV493D_WR_RES2] = res2;
    write_buffer[TLV493D_WR_MOD2] = mod2;

    return tlv493d_write(context);
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, TLV493D_I2C_ADDR);

    result = i2c_open(&g_context, argc, argv);
    if (result < 0)
        return result;

    result = setup(&g_context);
    if (result < 0)
        printf("ERROR: setup() failed %d\n", result);
    else
        loop(&g_context);

    i2c_close(&g_context);
    return result;
}
