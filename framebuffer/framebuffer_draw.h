// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Monochrome (1-Bit Per Pixel) 8-byte x 8-bit Framebuffer Draw Functions

#ifndef FRAMEBUFFER_DRAW_H
#define FRAMEBUFFER_DRAW_H

#include <stddef.h>
#include <stdint.h>

void set_byte(uint8_t* bytes, int index, uint8_t value);
void and_out_of_byte(uint8_t* bytes, int index, uint8_t mask);
void or_into_byte(uint8_t* bytes, int index, uint8_t mask);

void fill_bytes(uint8_t* bytes, int index, int count, uint8_t value);
void copy_bytes(uint8_t* bytes, int index, int count, const uint8_t* data);
void clear_pixel(void* fb, int pixel);
void set_pixel(void* fb, int pixel);

void clear_pixels(void* fb, int pixel, int n);
void set_pixels(void* fb, int pixel, int n);
void copy_pixels(void* fb, int pixel, int n, const uint8_t* data);
void draw_line_h(void* fb, int pixel, int w);
void copy_line_h(void* fb, int pixel, int w, const uint8_t* data);
void draw_lines_h(void* fb, int pixel, int w, int h);

void copy_lines_h(void* fb, int pixel, int w, int h, const uint8_t* data);
void draw_line_v(void* fb, int pixel, int h);

void draw_rect(void* fb, int x, int y, int w, int h);
void fill_rect(void* fb, int x, int y, int w, int h);

void copy_rect(void* fb, int x, int y, int w, int h, const uint8_t* data);
void draw_font_8x8(void* fb, int x, int y, const uint8_t* font);
void draw_circle(void* fb, int x, int y, int r);
void fill_rows(void* fb, uint8_t value);

void fill_screen(void* fb, uint8_t value);
void clear_screen(void* fb);

#endif // FRAMEBUFFER_DRAW_H
