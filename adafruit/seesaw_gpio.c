// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Adafruit I2C Stemma QT Seesaw GPIO Module
// https://github.com/adafruit/Adafruit_Seesaw/blob/master/examples/encoder/encoder_basic/encoder_basic.ino
//

#include <stdio.h>
#include <stdlib.h>
#include "seesaw_gpio.h"
#include "i2c_io.h"

int seesaw_gpio_set_mode(void* context, uint32_t bulk_mask, uint8_t mode) {
    uint8_t command[6] = {
        (uint8_t)SS_GPIO,
        (uint8_t)SS_GPIO_DIRSET_BULK,
        (uint8_t)(bulk_mask >> 24),
        (uint8_t)(bulk_mask >> 16),
        (uint8_t)(bulk_mask >>  8),
        (uint8_t)(bulk_mask >>  0)
    };
    size_t size = sizeof(command);

    if (mode == SS_GPIO_OUTPUT)
        return i2c_write(context, command, size);
    
    if (mode > SS_GPIO_INPUT_PULLDOWN)
        return SS_GPIO_INVALID_PIN_MODE;

    command[1] = (uint8_t)SS_GPIO_DIRCLR_BULK;
    if (i2c_write(context, command, size) < 0)
        return SS_GPIO_DIRCLR_BULK_ERROR;

    if (mode == SS_GPIO_INPUT)
        return SS_SUCCESS;

    command[1] = (uint8_t)SS_GPIO_PULLENSET;
    if (i2c_write(context, command, size) < 0)
        return SS_GPIO_PULLENSET_ERROR;

    if (mode == SS_GPIO_INPUT_PULLUP)
        command[1] = (uint8_t)SS_GPIO_BULK_SET;
    else
        command[1] = (uint8_t)SS_GPIO_BULK_CLR;

    return i2c_write(context, command, size);
}

int seesaw_gpio_set_interrupt(void* context, uint32_t bulk_mask, bool enable) {
    uint8_t command[6] = {
        (uint8_t)SS_GPIO,
        (uint8_t)(enable ? SS_GPIO_INTENSET : SS_GPIO_INTENCLR),
        (uint8_t)(bulk_mask >> 24),
        (uint8_t)(bulk_mask >> 16),
        (uint8_t)(bulk_mask >>  8),
        (uint8_t)(bulk_mask >>  0)
    };
    return i2c_write(context, command, sizeof(command));
}

int seesaw_gpio_get_bulk(void* context, uint32_t bulk_mask, uint32_t* value) {
    if (seesaw_read_u32_be(context, SS_GPIO, SS_GPIO_BULK, value) < 0)
        return -1;

    if (value)
        *value &= bulk_mask;

    return 0;
}

int seesaw_gpio_setup(void* context, uint32_t bulk_mask, uint8_t mode) {
    return seesaw_gpio_set_mode(context, bulk_mask, mode);
}
