// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Seesaw Encoder Module
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/examples/encoder/encoder_basic/encoder_basic.ino
//

#include <stdio.h>
#include <stdlib.h>
#include "seesaw_adc.h"
#include "seesaw_gpio.h"
#include "i2c_io.h"

int seesaw_adc_get_status(void* context, uint8_t* status) {
    return seesaw_read_u8(context, SS_ADC, SS_ADC_STATUS, status);
}

int seesaw_adc_set_interrupt(void* context, bool enable) {
    int reg = enable ? SS_ADC_INTENSET : SS_ADC_INTENCLR;
    return i2c_write_module_u8(context, SS_ADC, reg, 0x01);
}

int seesaw_adc_set_window_mode(void* context, uint8_t mode) {
    return i2c_write_module_u8(context, SS_ADC, SS_ADC_WINMODE, mode);
}

int seesaw_adc_set_window_threshold(void* context, uint32_t threshold) {
    return i2c_write_module_u32_be(context, SS_ADC, SS_ADC_WINTHRESH, threshold);
}

int seesaw_adc_get_channel(void* context, int channel, uint16_t* value) {
    return seesaw_read_u16_be(context, SS_ADC, SS_ADC_CHANNEL(channel), value);
}

int seesaw_adc_setup(void* context) {
    uint8_t status;
    if (seesaw_adc_get_status(context, &status) < 0)
        return SS_ADC_GET_STATUS_ERROR;

    printf("ADC Status: 0x%02x\n", status);

    return SS_SUCCESS;
}
