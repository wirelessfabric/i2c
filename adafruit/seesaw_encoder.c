// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Seesaw Encoder Module
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/examples/encoder/encoder_basic/encoder_basic.ino
//

#include <stdio.h>
#include <stdlib.h>
#include "seesaw_encoder.h"
#include "i2c_io.h"

int seesaw_encoder_set_position(void* context, int encoder, int32_t position) {
    return i2c_write_module_s32_be(context, SS_ENCODER, SS_ENCODER_POSITION + encoder, position);
}

int seesaw_encoder_get_position(void* context, int encoder, int32_t* position) {
    return seesaw_read_s32_be(context, SS_ENCODER, SS_ENCODER_POSITION + encoder, position);
}

int seesaw_encoder_set_interrupt(void* context, int encoder, bool enable) {
    int reg = enable ? SS_ENCODER_INTENSET : SS_ENCODER_INTENCLR;
    return i2c_write_module_u8(context, SS_ENCODER, reg + encoder, 0x01);
}
