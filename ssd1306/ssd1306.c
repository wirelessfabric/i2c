// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Solomon Systech SSD1306
// 128 x 64 Dot Matrix OLED/PLED Segment/Common Driver with Controller
//
// Data Sheet: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
//
// $ gcc -I../framebuffer
//       -I../font
//       ssd1306.c
//       ../font/font_8x8_mono.c
//       ../font/font_8x8_rotate.c
//       ../framebuffer/framebuffer_draw.c
//       ../framebuffer/framebuffer.c
//       -o ssd1306
// $ ./ssd1306

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#define __USE_MISC 1 // usleep
#include <unistd.h>
#include <time.h>    // sleep
#include "framebuffer.h"
#include "framebuffer_draw.h"
#include "font_8x8_mono.h"
#include "font_8x8_rotate.h"

static void show_circles(void* fb) {
    clear_screen(fb);
    sleep(3);
    int x=64, y=24;
    for (int r=1; r < y; r++) {
        clear_screen(fb);
        draw_circle(fb, x, y, r);
        fb_show(fb);
        usleep(100);
    }
    sleep(3);
}

static void show_font_8x8_mono(void* fb) {
    clear_screen(fb);
    for (int r=40, c=32; c < 128; r -= 8) {
        for (int i=0; i < fb_horizontal_pixels(); i += 8) {
            draw_font_8x8(fb, i, r, font_8x8_mono(c++));
        }
        fb_show(fb);
        sleep(1);
    }
}

static void show_font_8x8_90(void* fb) {
    clear_screen(fb);
    for (int r=40, c=32; c < 128; r -= 8) {
        for (int i=0; i < fb_horizontal_pixels(); i += 8) {
            uint8_t font[8];
            font_8x8_rotate_90(font, font_8x8_mono(c++));
            draw_font_8x8(fb, i, r, font);
        }
        fb_show(fb);
        sleep(1);
    }
}

static void show_font_8x8_180(void* fb) {
    clear_screen(fb);
    for (int r=40, c=32; c < 128; r -= 8) {
        for (int i=0; i < fb_horizontal_pixels(); i += 8) {
            uint8_t font[8];
            font_8x8_rotate_180(font, font_8x8_mono(c++));
            draw_font_8x8(fb, i, r, font);
        }
        fb_show(fb);
        sleep(1);
    }
}

static void show_font_8x8_270(void* fb) {
    clear_screen(fb);
    for (int r=40, c=32; c < 128; r -= 8) {
        for (int i=0; i < fb_horizontal_pixels(); i += 8) {
            uint8_t font[8];
            font_8x8_rotate_270(font, font_8x8_mono(c++));
            draw_font_8x8(fb, i, r, font);
        }
        fb_show(fb);
        sleep(1);
    }
}

static void show_line_crawl(void* fb) {
    int w = 16;
    int lines = fb_horizontal_pixels() - w;
    for (int i=0; i < lines; i++) {
        clear_screen(fb);
        draw_line_h(fb, i, w);
        fb_show(fb);
        usleep(500);
    }
}

static void show_rect(void* fb) {
    clear_screen(fb);
    fill_rect(fb, 0, 0, 16, 8);
    draw_rect(fb, 8, 16, 16, 8);
    fill_rect(fb, 16, 32, 16, 8);
    draw_rect(fb, 24, 48, 16, 8);
    fb_show(fb);
    sleep(1);
}

static void show_random_pixels(void* fb) {
    clear_screen(fb);
    int count = fb_pixels();
    for (int i=0; i < count; i++) {
        set_pixel(fb, rand() % count);
        fb_show(fb);
        usleep(1);
    }
    sleep(1);
}

static void show_random_bytes(void* fb) {
    clear_screen(fb);
    int count = fb_bytes();
    for (int i=0; i < count; i++) {
        set_byte(fb, rand() % count, 0xff);
        fb_show(fb);
        usleep(1);
    }
    sleep(1);
}

static void show_fill(void* fb) {
    clear_screen(fb);
    fill_rows(fb, 0xff);
    fb_show(fb);
    sleep(1);
}

int main(int argc, char *argv[]) {
    int file = fb_open(FBDEV_NAME, argc, argv);
    if (file < 0)
        return -1;

    void* fb = fb_map(file);
    if (!fb) {
        fb_close(file);
        return -2;
    }

    show_circles(fb);

    show_font_8x8_mono(fb);
    show_font_8x8_90(fb);
    show_font_8x8_180(fb);
    show_font_8x8_270(fb);

    clear_pixels(fb, 0, fb_horizontal_pixels());

    show_line_crawl(fb);
    show_rect(fb);

    show_random_pixels(fb);
    show_random_bytes(fb);

    show_fill(fb);

    fb_unmap(fb);
    fb_close(file);
    return 0;
}
