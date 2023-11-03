// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Monochrome (1-Bit Per Pixel) 8-byte x 8-bit Framebuffer Draw Functions

#include <math.h>    // sqrtf
#include "framebuffer.h"
#include "framebuffer_draw.h"

void set_byte(uint8_t* bytes, int index, uint8_t value) {
    bytes[index] = value;
}

void and_out_of_byte(uint8_t* bytes, int index, uint8_t mask) {
    bytes[index] &= ~mask;
}

void or_into_byte(uint8_t* bytes, int index, uint8_t mask) {
    bytes[index] |= mask;
}

void fill_bytes(uint8_t* bytes, int index, int count, uint8_t value) {
    bytes += index;
    for (int i=0; i < count; i++)
        set_byte(bytes, i, value);
}

void copy_bytes(uint8_t* bytes, int index, int count, const uint8_t* data) {
    bytes += index;
    for (int i=0; i < count; i++)
        set_byte(bytes, i, data[i]);
}

void clear_pixel(void* fb, int pixel) {
    int index = pixel >> 3;
    uint8_t bit = (uint8_t)(1 << (pixel - (index << 3)));
    and_out_of_byte(fb, index, bit);
}

void set_pixel(void* fb, int pixel) {
    int index = pixel >> 3;
    uint8_t bit = (uint8_t)(1 << (pixel - (index << 3)));
    or_into_byte(fb, index, bit);
}

void clear_pixels(void* fb, int pixel, int n) {
    for (int i=0; i < n; i++)
        clear_pixel(fb, pixel + i);
}

void set_pixels(void* fb, int pixel, int n) {
    int a = pixel & 7;
    if (a) {
        uint8_t mask[8] = {
            0x00, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80
        };
        or_into_byte(fb, pixel >> 3, n < 8 ? mask[8 - a] : mask[a]);
        a = 8 - a;
        pixel += a;
    }

    int b = (n - a) & ~7;
    if (b) {
        fill_bytes(fb, pixel >> 3, b >> 3, 0xff);
        pixel += b;
    }

    int c = n - a - b;
    if (c) {
        uint8_t mask[8] = {
            0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f
        };
        or_into_byte(fb, pixel >> 3, mask[c]);
    }
}

void copy_pixels(void* fb, int pixel, int n, const uint8_t* data) {
    // TODO: handle unaligned pixels before and after byte-aligned pixels
    copy_bytes(fb, pixel >> 3, n >> 3, data);
}

void draw_line_h(void* fb, int pixel, int w) {
    set_pixels(fb, pixel, w);
}

void copy_line_h(void* fb, int pixel, int w, const uint8_t* data) {
    copy_pixels(fb, pixel, w, data);
}

void draw_lines_h(void* fb, int pixel, int w, int h) {
    for (int i=0; i < h; i++) {
        draw_line_h(fb, pixel, w);
        pixel += fb_horizontal_pixels();
    }
}

void copy_lines_h(void* fb, int pixel, int w, int h, const uint8_t* data) {
    for (int i=0; i < h; i++) {
        copy_line_h(fb, pixel, w, data);
        pixel += fb_horizontal_pixels();
        data += (w >> 3);
    }
}

void draw_line_v(void* fb, int pixel, int h) {
    for (int i=0; i < h; i++, pixel += fb_horizontal_pixels())
        set_pixel(fb, pixel);
}

void draw_rect(void* fb, int x, int y, int w, int h) {
    int bottom_left = x + y * fb_horizontal_pixels();
    int bottom_right = bottom_left + w;
    int top_left = bottom_left + h * fb_horizontal_pixels();

    draw_line_h(fb, bottom_left, w);
    draw_line_v(fb, bottom_left, h);
    draw_line_v(fb, bottom_right, h);
    draw_line_h(fb, top_left, w);
}

void fill_rect(void* fb, int x, int y, int w, int h) {
    int bottom_left = x + y * fb_horizontal_pixels();
    draw_lines_h(fb, bottom_left, w, h);
}

void copy_rect(void* fb, int x, int y, int w, int h, const uint8_t* data) {
    int bottom_left = x + y * fb_horizontal_pixels();
    copy_lines_h(fb, bottom_left, w, h, data);
}

void draw_font_8x8(void* fb, int x, int y, const uint8_t* font) {
    copy_rect(fb, x, y, 8, 8, font);
}

void draw_circle(void* fb, int x, int y, int r) {
    int h = fb_horizontal_pixels();
    int v = r * h;
    int o = x + y * h;

    set_pixel(fb, o + r);
    set_pixel(fb, o - r);
    set_pixel(fb, o + v);
    set_pixel(fb, o - v);

    int radius_squared = r * r;
    int up, dn;
    for (int a=1; a < r; a++) {
        int b = sqrtf(radius_squared - a * a);

        up = (y + b) * h;
        dn = (y - b) * h;
        set_pixel(fb, x + a + up);
        set_pixel(fb, x - a + up);
        set_pixel(fb, x + a + dn);
        set_pixel(fb, x - a + dn);

        up = (y + a) * h;
        dn = (y - a) * h;
        set_pixel(fb, x + b + up);
        set_pixel(fb, x - b + up);
        set_pixel(fb, x + b + dn);
        set_pixel(fb, x - b + dn);
    }
}

void fill_rows(void* fb, uint8_t value) {
    int rows = fb_vertical_pixels();
    int width = fb_horizontal_bytes();
    for (int index=0, r=0; r < rows; r++, index += width)
        fill_bytes(fb, index, width, value);
}

void fill_screen(void* fb, uint8_t value) {
    fill_bytes(fb, 0, fb_bytes(), value);
}

void clear_screen(void* fb) {
    fill_screen(fb, 0x00);
    fb_show(fb);
}
