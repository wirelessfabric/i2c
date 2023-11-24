// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Bosch Sensortec BME680/BME688 Combined Temperature, Humidity, Pressure, and Gas Sensor
// Data Sheet:
// https://cdn-shop.adafruit.com/product-files/3660/BME680.pdf
//
// $ gcc bme68x.c ../io/i2c_dev_io.c -I../io -lm -o bme68x
// $ ./bme68x

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bme68x.h"

static struct bme68x_coeffs {
    // temperature compensation values
    int T1;
    int T2;
    int T3;

    // pressure compensation values
    int64_t P1;
    int64_t P2;
    int64_t P3;
    int64_t P4;
    int64_t P5;
    int64_t P6;
    int64_t P7;
    int64_t P8;
    int64_t P9;
    int64_t P10;

    // humidity compensation values
    int H1;
    int H2;
    int H3;
    int H4;
    int H5;
    int H6;
    int H7;

    // gas compensation values
    int G1;
    int G2;
    int G3;
} coeffs;

static uint8_t variant = 0;
static int t_fine = 0;
static int t_fine_adjust = 0;

static inline float ctof(float celsius) {
    return (celsius * 1.8f) + 32.f;
}

static void bme68x_block_while_status(void* context, uint8_t bits, unsigned usec) {
    while ((i2c_read_u8(context, BME68X_REG_STATUS) & bits) != 0)
        i2c_sleep(usec);
}

static void bme68x_block_while_measuring(void* context, unsigned usec) {
    while ((i2c_read_u8(context, BME68X_REG_MEASUREMENT) & BME68X_MEASURING) != 0)
        i2c_sleep(usec);
}

static void bme68x_read_coeffs(void* context) {
    coeffs.T1 = (int)i2c_read_s16_le(context, BME68X_REG_T1);
    coeffs.T2 = (int)i2c_read_s16_le(context, BME68X_REG_T2);
    coeffs.T3 = (int)i2c_read_u8(context, BME68X_REG_T3);

    coeffs.P1 = (int64_t)i2c_read_u16_le(context, BME68X_REG_P1);
    coeffs.P2 = (int64_t)i2c_read_s16_le(context, BME68X_REG_P2);
    coeffs.P3 = (int64_t)i2c_read_u8(context, BME68X_REG_P3);
    coeffs.P4 = (int64_t)i2c_read_s16_le(context, BME68X_REG_P4);
    coeffs.P5 = (int64_t)i2c_read_s16_le(context, BME68X_REG_P5);
    coeffs.P6 = (int64_t)i2c_read_u8(context, BME68X_REG_P6);
    coeffs.P7 = (int64_t)i2c_read_u8(context, BME68X_REG_P7);
    coeffs.P8 = (int64_t)i2c_read_s16_le(context, BME68X_REG_P8);
    coeffs.P9 = (int64_t)i2c_read_s16_le(context, BME68X_REG_P9);
    coeffs.P10 = (int64_t)i2c_read_u8(context, BME68X_REG_P10);

    coeffs.H1 = (int)i2c_read_u16_le(context, BME68X_REG_H1);
    coeffs.H2 = (int)i2c_read_u8(context, BME68X_REG_H2);
    coeffs.H3 = (int)i2c_read_s8(context, BME68X_REG_H3);
    coeffs.H4 = (int)i2c_read_s8(context, BME68X_REG_H4);
    coeffs.H5 = (int)i2c_read_s8(context, BME68X_REG_H5);
    coeffs.H6 = (int)i2c_read_u8(context, BME68X_REG_H6);
    coeffs.H7 = (int)i2c_read_s8(context, BME68X_REG_H7);

    coeffs.G1 = (int)i2c_read_s8(context, BME68X_REG_G1);
    coeffs.G2 = (int)i2c_read_s16_le(context, BME68X_REG_G2);
    coeffs.G3 = (int)i2c_read_s8(context, BME68X_REG_G3);
}

static void bme68x_setup_weather_monitor(void* context) {
    i2c_write_reg_u8(context, BME68X_REG_CONTROL, BME68X_MODE_SLEEP);

    i2c_write_reg_u8(context, BME68X_REG_CONTROL_HUMIDITY, BME68X_OVERSCAN_HUMIDITY);

    i2c_write_reg_u8(context, BME68X_REG_CONFIG, BME68X_IIR_FILTER_OFF |
                                                 BME68X_STANDBY_MS_0_5 |
                                                 BME68X_SPI_3WIRE_OFF);
}

static int bme68x_compute_fine_temperature(uint32_t t) {
    if (t == 0x800000)
        return 0;

    // drop lowest 4 bits
    t >>= 4;

    int t1 = (int)(t >> 3) - (coeffs.T1 << 1);
    t1 = (t1 * coeffs.T2) >> 11;

    int t2 = (int)(t >> 4) - coeffs.T1;
    t2 = (((t2 * t2) >> 12) * coeffs.T3) >> 14;

    return t1 + t2 + t_fine_adjust;
}

static float bme68x_compute_temperature(uint32_t t) {
    t_fine = bme68x_compute_fine_temperature(t);
    return (float)((t_fine * 5 + 128) >> 8) / 100.f;
}

static void bme68x_monitor_weather(void* context) {
    i2c_write_reg_u8(context, BME68X_REG_CONTROL, BME68X_OVERSCAN_TEMPERATURE |
                                                  BME68X_OVERSCAN_PRESSURE |
                                                  BME68X_MODE_FORCE);

    bme68x_block_while_measuring(context, 2000);

    (void)i2c_read_u24_be(context, BME68X_REG_PRESSURE);
    (void)i2c_read_u16_be(context, BME68X_REG_HUMIDITY);

    uint32_t t = i2c_read_u24_be(context, BME68X_REG_TEMPERATURE);

    i2c_write_reg_u8(context, BME68X_REG_CONTROL, BME68X_MODE_SLEEP);

    float T = bme68x_compute_temperature(t);

    printf("Temperature: %5.2f C %5.2f F\n", T, ctof(T));
}

static void loop(void* context) {
    while (true) {
        bme68x_monitor_weather(context);
        i2c_sleep(2000000);
    }
}

static int setup(void* context) {
    uint8_t chip_id;
    int result = i2c_read_reg_u8(context, BME68X_REG_CHIP_ID, &chip_id);
    if (result == I2C_WRITE_ERROR) {
        printf("ERROR: setup() no response from sensor\n");
        return BME68X_NO_RESPONSE;
    }
    else if (result < 0) {
        printf("ERROR: setup() unable to read sensor chip id\n");
        return BME68X_READ_CHIP_ID_ERROR;
    }
    else if (chip_id != BME68X_CHIP_ID) {
        printf("ERROR: setup() unexpected chip id 0x%02x\n", chip_id);
        return BME68X_INVALID_CHIP_ID;
    }

    if (i2c_read_reg_u8(context, BME68X_REG_VARIANT, &variant) < 0) {
        printf("ERROR: setup() unable to read sensor variant\n");
        return BME68X_READ_VARIANT_ERROR;
    }
    else if (variant > BME68X_VARIANT_MAX) {
        printf("ERROR: setup() unexpected variant 0x%02x\n", variant);
        return BME68X_INVALID_CHIP_ID;
    }

    if (i2c_write_reg_u8(context, BME68X_REG_SOFT_RESET, 0xB6) < 0) {
        printf("ERROR: setup() software reset\n");
        return BME68X_RESET_ERROR;
    }

    i2c_sleep(4000);
    bme68x_block_while_status(context, BME68X_STATUS_IM_UPDATE, 10000);
    bme68x_read_coeffs(context);
    bme68x_setup_weather_monitor(context);
    return BME68X_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, BME68X_I2C_ADDR);

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
