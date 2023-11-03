// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Seesaw Encoder Module
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/Adafruit_seesaw.h
//

#ifndef SEESAW_ADC_H_
#define SEESAW_ADC_H_

#include "adafruit_seesaw.h"

// Seesaw Analog to Digital Converter (ADC) module definitions
#define SS_ADC_STATUS           0x00
#define SS_ADC_INTENSET         0x01
#define SS_ADC_INTENCLR         0x02
#define SS_ADC_WINMODE          0x04
#define SS_ADC_WINTHRESH        0x05
#define SS_ADC_CHANNEL(c)       (uint8_t)(0x07 + (c))

// NeoSlider product definitions
#define NEOSLIDER_ADC_CHANNEL   18

int seesaw_adc_get_status(void* context, uint8_t* status);
int seesaw_adc_set_interrupt(void* context, bool enable);
int seesaw_adc_set_window_mode(void* context, uint8_t mode);
int seesaw_adc_set_window_threshold(void* context, uint32_t threshold);
int seesaw_adc_get_channel(void* context, int channel, uint16_t* value);
int seesaw_adc_setup(void* context);

#endif // SEESAW_ADC_H_
