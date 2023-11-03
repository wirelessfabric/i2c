// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Seesaw Encoder Module
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/Adafruit_seesaw.h
//

#ifndef SEESAW_ENCODER_H_
#define SEESAW_ENCODER_H_

#include "adafruit_seesaw.h"

// Seesaw encoder module definitions
#define SS_ENCODER_STATUS   0x00
#define SS_ENCODER_INTENSET 0x10
#define SS_ENCODER_INTENCLR 0x20
#define SS_ENCODER_POSITION 0x30
#define SS_ENCODER_DELTA    0x40

int seesaw_encoder_set_position(void* context, int encoder, int32_t position);
int seesaw_encoder_get_position(void* context, int encoder, int32_t* position);
int seesaw_encoder_set_interrupt(void* context, int encoder, bool enable);

#endif // SEESAW_ENCODER_H_
