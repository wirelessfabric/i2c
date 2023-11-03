// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Monochrome Font for Solomon Systech SSD1306
// 128 x 64 Dot Matrix OLED/PLED Segment/Common Driver with Controller

#include <stdio.h>
#include <stdint.h>

void font_8x8_rotate_90(uint8_t rect[8], const uint8_t font[8]) {
    rect[0] = (uint8_t)((font[0] & 0x80) >> 0) |
              (uint8_t)((font[1] & 0x80) >> 1) |
              (uint8_t)((font[2] & 0x80) >> 2) |
              (uint8_t)((font[3] & 0x80) >> 3) |
              (uint8_t)((font[4] & 0x80) >> 4) |
              (uint8_t)((font[5] & 0x80) >> 5) |
              (uint8_t)((font[6] & 0x80) >> 6) |
              (uint8_t)((font[7] & 0x80) >> 7);

    rect[1] = (uint8_t)((font[0] & 0x40) >> 6) |
              (uint8_t)((font[1] & 0x40) >> 5) |
              (uint8_t)((font[2] & 0x40) >> 4) |
              (uint8_t)((font[3] & 0x40) >> 3) |
              (uint8_t)((font[4] & 0x40) >> 2) |
              (uint8_t)((font[5] & 0x40) >> 1) |
              (uint8_t)((font[6] & 0x40) << 0) |
              (uint8_t)((font[7] & 0x40) << 1);

    rect[2] = (uint8_t)((font[0] & 0x20) >> 5) |
              (uint8_t)((font[1] & 0x20) >> 4) |
              (uint8_t)((font[2] & 0x20) >> 3) |
              (uint8_t)((font[3] & 0x20) >> 2) |
              (uint8_t)((font[4] & 0x20) >> 1) |
              (uint8_t)((font[5] & 0x20) << 0) |
              (uint8_t)((font[6] & 0x20) << 1) |
              (uint8_t)((font[7] & 0x20) << 2);

    rect[3] = (uint8_t)((font[0] & 0x10) >> 4) |
              (uint8_t)((font[1] & 0x10) >> 3) |
              (uint8_t)((font[2] & 0x10) >> 2) |
              (uint8_t)((font[3] & 0x10) >> 1) |
              (uint8_t)((font[4] & 0x10) << 0) |
              (uint8_t)((font[5] & 0x10) << 1) |
              (uint8_t)((font[6] & 0x10) << 2) |
              (uint8_t)((font[7] & 0x10) << 3);

    rect[4] = (uint8_t)((font[0] & 0x08) >> 3) |
              (uint8_t)((font[1] & 0x08) >> 2) |
              (uint8_t)((font[2] & 0x08) >> 1) |
              (uint8_t)((font[3] & 0x08) << 0) |
              (uint8_t)((font[4] & 0x08) << 1) |
              (uint8_t)((font[5] & 0x08) << 2) |
              (uint8_t)((font[6] & 0x08) << 3) |
              (uint8_t)((font[7] & 0x08) << 4);

    rect[5] = (uint8_t)((font[0] & 0x04) >> 2) |
              (uint8_t)((font[1] & 0x04) >> 1) |
              (uint8_t)((font[2] & 0x04) << 0) |
              (uint8_t)((font[3] & 0x04) << 1) |
              (uint8_t)((font[4] & 0x04) << 2) |
              (uint8_t)((font[5] & 0x04) << 3) |
              (uint8_t)((font[6] & 0x04) << 4) |
              (uint8_t)((font[7] & 0x04) << 5);

    rect[6] = (uint8_t)((font[0] & 0x02) >> 1) |
              (uint8_t)((font[1] & 0x02) << 0) |
              (uint8_t)((font[2] & 0x02) << 1) |
              (uint8_t)((font[3] & 0x02) << 2) |
              (uint8_t)((font[4] & 0x02) << 3) |
              (uint8_t)((font[5] & 0x02) << 4) |
              (uint8_t)((font[6] & 0x02) << 5) |
              (uint8_t)((font[7] & 0x02) << 6);

    rect[7] = (uint8_t)((font[0] & 0x01) << 0) |
              (uint8_t)((font[1] & 0x01) << 1) |
              (uint8_t)((font[2] & 0x01) << 2) |
              (uint8_t)((font[3] & 0x01) << 3) |
              (uint8_t)((font[4] & 0x01) << 4) |
              (uint8_t)((font[5] & 0x01) << 5) |
              (uint8_t)((font[6] & 0x01) << 6) |
              (uint8_t)((font[7] & 0x01) << 7);
}

void font_8x8_rotate_180(uint8_t rect[8], const uint8_t font[8]) {
    uint8_t rect90[8];
    font_8x8_rotate_90(rect90, font);
    font_8x8_rotate_90(rect, rect90);
}

void font_8x8_rotate_270(uint8_t rect[8], const uint8_t font[8]) {
    uint8_t rect180[8];
    font_8x8_rotate_180(rect180, font);
    font_8x8_rotate_90(rect, rect180);
}

void font_8x8_print(uint8_t font[8]) {
    printf("0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,\n",
           font[0], font[1], font[2], font[3],
           font[4], font[5], font[6], font[7]);
}
