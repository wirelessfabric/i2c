// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit Seesaw Platform
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/Adafruit_seesaw.cpp
// https://learn.adafruit.com/adafruit-seesaw-atsamd09-breakout
//

#include <stdio.h>
#include <stdlib.h>
#include "adafruit_seesaw.h"

#define SS_DELAY 10000

int seesaw_read_u8(context* context, int base, int reg, uint8_t* value) {
    unsigned int old_delay = i2c_delay_usec(context);
    if (old_delay < SS_DELAY)
        i2c_set_delay_usec(context, SS_DELAY);

    int result = i2c_read_module_u8(context, base, reg, value);

    if (old_delay != SS_DELAY)
        i2c_set_delay_usec(context, old_delay);

    i2c_sleep(SS_DELAY);
    return result;
}

int seesaw_read_u16_be(context* context, int base, int reg, uint16_t* value) {
    unsigned int old_delay = i2c_delay_usec(context);
    if (old_delay < SS_DELAY)
        i2c_set_delay_usec(context, SS_DELAY);

    int result = i2c_read_module_u16_be(context, base, reg, value);

    if (old_delay != SS_DELAY)
        i2c_set_delay_usec(context, old_delay);

    i2c_sleep(SS_DELAY);
    return result;
}

int seesaw_read_u32_be(context* context, int base, int reg, uint32_t* value) {
    unsigned int old_delay = i2c_delay_usec(context);
    if (old_delay < SS_DELAY)
        i2c_set_delay_usec(context, SS_DELAY);

    int result = i2c_read_module_u32_be(context, base, reg, value);

    if (old_delay != SS_DELAY)
        i2c_set_delay_usec(context, old_delay);

    i2c_sleep(SS_DELAY);
    return result;
}

int seesaw_read_s32_be(context* context, int base, int reg, int32_t* value) {
    return seesaw_read_u32_be(context, base, reg, (uint32_t*)value);
}

///////////////////////////////////////////////////////////////////////////////

int seesaw_sw_reset(void* context) {
    return i2c_write_module_u8(context, SS_STATUS, SS_STATUS_SW_RESET, 0xFF);
}

int seesaw_get_hw_id(void* context, uint8_t* hw_id) {
    return seesaw_read_u8(context, SS_STATUS, SS_STATUS_HW_ID, hw_id);
}

int seesaw_get_fw_version(void* context, uint32_t* fw_version) {
    return seesaw_read_u32_be(context, SS_STATUS, SS_STATUS_FW_VERSION, fw_version);
}

int seesaw_get_fw_options(void* context, uint32_t* fw_options) {
    return seesaw_read_u32_be(context, SS_STATUS, SS_STATUS_FW_OPTIONS, fw_options);
}

int seesaw_get_temperature(void* context, float* temperature) {
    uint32_t t = 0;
    if (seesaw_read_u32_be(context, SS_STATUS, SS_STATUS_TEMPERATURE, &t) < 0)
        return SS_GET_TEMPERATURE_ERROR;

    if (temperature)
        *temperature = (1.f / (float)(1UL << 16)) * (float)t;

    return SS_SUCCESS;
}

int seesaw_setup(void* context, uint8_t id, uint32_t version, uint32_t options) {
    uint8_t hw_id = 0;
    if (seesaw_get_hw_id(context, &hw_id) < 0) {
        printf("seesaw_setup: seesaw_get_hw_id() failed\n");
        return SS_GET_HW_ID_ERROR;
    }

    if (hw_id != id) {
        printf("seesaw_setup: Invalid hardware id 0x%02x\n", (uint32_t)hw_id);
        return SS_INVALID_HW_ID;
    }

    if (version) {
        uint32_t fw_version = 0;
        if (seesaw_get_fw_version(context, &fw_version) < 0) {
            printf("seesaw_setup: seesaw_get_fw_version() failed\n");
            return SS_GET_FW_VERSION_ERROR;
        }

        fw_version >>= 16;
        fw_version &= 0xFFFF;

        if (fw_version < version) {
            printf("seesaw_setup: Invalid firmware version 0x%x\n", fw_version);
            return SS_INVALID_FW_VERSION;
        }
    }

    if (options) {
        uint32_t fw_options = 0;
        if (seesaw_get_fw_options(context, &fw_options) < 0) {
            printf("seesaw_setup(): seesaw_get_fw_options() failed\n");
            return SS_GET_FW_OPTIONS_ERROR;
        }

        if ((fw_options & options) != options) {
            printf("seesaw_setup(): missing firmware options 0x%x\n", fw_options);
            return SS_NO_FW_VERSION;
        }
    }

    return SS_SUCCESS;
}

int seesaw_samd09_setup(void* context, uint32_t version, uint32_t options) {
    return seesaw_setup(context, SS_SAMD09_HW_ID, version, options);
}

int seesaw_tiny8x7_setup(void* context, uint32_t version, uint32_t options) {
    return seesaw_setup(context, SS_TINY8X7_HW_ID, version, options);
}
