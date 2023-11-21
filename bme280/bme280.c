// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Bosch Sensortec BME280 Combined Temperature, Humidity, and Pressure Sensor
// Data Sheet:
// https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
//
// $ gcc bme280.c ../io/i2c_dev_io.c -I../io -lm -o bme280
// $ ./bme280

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bme280.h"

static struct bme280_coeffs {
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

    // humidity compensation values
    int H1;
    int H2;
    int H3;
    int H4;
    int H5;
    int H6;
} coeffs;

static int t_fine = 0;
static int t_fine_adjust = 0;
static float p_sea_level = 1013.25f; // hPa

static inline float ctof(float celsius) {
    return (celsius * 1.8f) + 32.f;
}

static void bme280_block_while_status(void* context, uint8_t bits, unsigned usec) {
    while ((i2c_read_u8(context, BME280_REG_STATUS) & bits) != 0)
        i2c_sleep(usec);
}

static void bme280_read_coeffs(void* context) {
    coeffs.T1 = (int)i2c_read_u16_le(context, BME280_REG_T1);
    coeffs.T2 = (int)i2c_read_s16_le(context, BME280_REG_T2);
    coeffs.T3 = (int)i2c_read_s16_le(context, BME280_REG_T3);

    coeffs.P1 = (int64_t)i2c_read_u16_le(context, BME280_REG_P1);
    coeffs.P2 = (int64_t)i2c_read_s16_le(context, BME280_REG_P2);
    coeffs.P3 = (int64_t)i2c_read_s16_le(context, BME280_REG_P3);
    coeffs.P4 = (int64_t)i2c_read_s16_le(context, BME280_REG_P4);
    coeffs.P5 = (int64_t)i2c_read_s16_le(context, BME280_REG_P5);
    coeffs.P6 = (int64_t)i2c_read_s16_le(context, BME280_REG_P6);
    coeffs.P7 = (int64_t)i2c_read_s16_le(context, BME280_REG_P7);
    coeffs.P8 = (int64_t)i2c_read_s16_le(context, BME280_REG_P8);
    coeffs.P9 = (int64_t)i2c_read_s16_le(context, BME280_REG_P9);

    coeffs.H1 = (int)i2c_read_u8(context, BME280_REG_H1);
    coeffs.H2 = (int)i2c_read_s16_le(context, BME280_REG_H2);
    coeffs.H3 = (int)i2c_read_u8(context, BME280_REG_H3);
    uint8_t e4 = i2c_read_u8(context, BME280_REG_E4);
    uint8_t e5 = i2c_read_u8(context, BME280_REG_E5);
    uint8_t e6 = i2c_read_u8(context, BME280_REG_E6);
    coeffs.H4 = ((int)e4 << 4) | (e5 & 0xF);
    coeffs.H5 = ((int)e6 << 4) | (e5 >> 4);
    coeffs.H6 = (int)i2c_read_u8(context, BME280_REG_H6);

    // pre-shift recurring shifts
    coeffs.P4 <<= 35;
    coeffs.P7 <<= 4;
    coeffs.H4 <<= 20;
}

static void bme280_setup_weather_monitor(void* context) {
    i2c_write_reg_u8(context, BME280_REG_CONTROL, BME280_MODE_SLEEP);

    i2c_write_reg_u8(context, BME280_REG_CONTROL_HUMIDITY, BME280_OVERSCAN_HUMIDITY);

    i2c_write_reg_u8(context, BME280_REG_CONFIG, BME280_IIR_FILTER_OFF |
                                                 BME280_STANDBY_MS_0_5 |
                                                 BME280_SPI_3WIRE_OFF);
}

static int bme280_compute_fine_temperature(uint32_t t) {
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

static float bme280_compute_temperature(uint32_t t) {
    t_fine = bme280_compute_fine_temperature(t);
    return (float)((t_fine * 5 + 128) >> 8) / 100.f;
}

static float bme280_compute_humidity(uint16_t h) {
    if (h == 0x8000)
        return 0.f;

    int h1 = t_fine - 76800;
    int h2 = (h1 * coeffs.H6) >> 10;
    int h3 = (h1 * coeffs.H3) >> 11;
    int h4 = ((h2 * (h3 + 32768)) >> 10) + 2097152;
    int h5 = ((int)h << 14) - coeffs.H4 - (h1 * coeffs.H5) + 16384;

    h3 = (h5 >> 15) * ((h4 * coeffs.H2 + 8192) >> 14);
    h4 = h3 >> 15;
    h4 = (h4 * h4) >> 7;
    h5 = h3 - ((h4 * coeffs.H1) >> 4);

    if (h5 < 0)
        h5 = 0;
    else if (h5 > 419430400)
        h5 = 419430400;

    return (float)(h5 >> 12) / 1024.f;
}

static float bme280_compute_pressure(uint32_t p) {
    if (p == 0x800000)
        return 0.f;

    // drop lowest 4 bits
    p >>= 4;

    int64_t p1 = (int64_t)t_fine - 128000;
    int64_t p2 = p1 * p1;
    int64_t p3 = p2 * coeffs.P6;
    int64_t p4 = (int64_t)1 << 47;

    p3 += (p1 * coeffs.P5) << 17;
    p3 += coeffs.P4;

    p1 = ((p2 * coeffs.P3) >> 8) + ((p1 * coeffs.P2) << 12);
    p1 = ((p4 + p1) * coeffs.P1) >> 33;
    if (p1 == 0)
        return 0.f;

    p4 = 1048576 - p;
    p4 = (((p4 << 31) - p3) * 3125) / p1;
    p2 = p4 >> 13;
    p1 = (coeffs.P9 * p2 * p2) >> 25;
    p3 = (coeffs.P8 * p4) >> 19;
    p4 = ((p4 + p1 + p3) >> 8) + coeffs.P7;

    return (float)p4 / 25600.f;
}

static float bme280_compute_altitude(float p) {
    return 44330.f * (1.f - (float)pow(p / p_sea_level, 0.1903f));
}

static void bme280_monitor_weather(void* context) {
    i2c_write_reg_u8(context, BME280_REG_CONTROL, BME280_OVERSCAN_TEMPERATURE |
                                             BME280_OVERSCAN_PRESSURE |
                                             BME280_MODE_FORCE);

    bme280_block_while_status(context, BME280_STATUS_MEASURING, 2000);

    uint32_t p = i2c_read_u24_be(context, BME280_REG_PRESSURE);
    uint32_t t = i2c_read_u24_be(context, BME280_REG_TEMPERATURE);
    uint16_t h = i2c_read_u16_be(context, BME280_REG_HUMIDITY);

    i2c_write_reg_u8(context, BME280_REG_CONTROL, BME280_MODE_SLEEP);

    float T = bme280_compute_temperature(t);
    float H = bme280_compute_humidity(h);
    float P = bme280_compute_pressure(p);
    float A = bme280_compute_altitude(P);

    printf("Temperature: %5.2f C %5.2f F\n", T, ctof(T));
    printf("Humidity: %5.2f %%\n", H);
    printf("Pressure: %6.2f hPa\n", P);
    printf("Altitude: %6.2f meters\n\n", A);
}

static void loop(void* context) {
    while (true) {
        bme280_monitor_weather(context);
        i2c_sleep(2000000);
    }
}

static int setup(void* context) {
    uint8_t chip_id;
    int result = i2c_read_reg_u8(context, BME280_REG_CHIP_ID, &chip_id);
    if (result == I2C_WRITE_ERROR) {
        printf("ERROR: setup() no response from sensor\n");
        return BME280_NO_RESPONSE;
    }
    else if (result < 0) {
        printf("ERROR: setup() unable to read sensor chip id\n");
        return BME280_READ_CHIP_ID_ERROR;
    }
    else if (chip_id != BME280_CHIP_ID) {
        printf("ERROR: setup() unexpected chip id 0x%02x\n", chip_id);
        return BME280_INVALID_CHIP_ID;
    }

    if (i2c_write_reg_u8(context, BME280_REG_SOFT_RESET, 0xB6) < 0) {
        printf("ERROR: setup() software reset\n");
        return BME280_RESET_ERROR;
    }

    i2c_sleep(4000);
    bme280_block_while_status(context, BME280_STATUS_IM_UPDATE, 10000);
    bme280_read_coeffs(context);
    bme280_setup_weather_monitor(context);
    return BME280_SUCCESS;
}

static context g_context = {0};

int main(int argc, char *argv[]) {
    int result;

    i2c_set_addr(&g_context, BME280_I2C_ADDR);

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
