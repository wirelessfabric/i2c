// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Monochrome (1-Bit Per Pixel) 8-byte x 8-bit Framebuffer

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stddef.h>
#include <stdint.h>

#define FBDEV_NAME  "/dev/fb1"

int fb_bits_per_pixel(void);

int fb_horizontal_pixels(void);
int fb_vertical_pixels(void);
int fb_pixels(void);

int fb_horizontal_bytes(void);
int fb_vertical_bytes(void);
int fb_bytes(void);
size_t fb_size(void);

int fb_open(const char* fbdev, int argc, char* argv[]);
void* fb_map(int file);
void fb_show(void* fb);
void fb_unmap(void* fb);
void fb_close(int file);

#endif // FRAMEBUFFER_H
