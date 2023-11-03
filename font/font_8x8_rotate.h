// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Monochrome (1-Bit Per Pixel) 8-byte x 8-bit Font
// Rotation Functions

#ifndef FONT_8x8_ROTATE_H
#define FONT_8x8_ROTATE_H

#include <stdint.h>

void font_8x8_rotate_90(uint8_t rect[8], const uint8_t font[8]);
void font_8x8_rotate_180(uint8_t rect[8], const uint8_t font[8]);
void font_8x8_rotate_270(uint8_t rect[8], const uint8_t font[8]);

#endif // FONT_8x8_ROTATE_H
