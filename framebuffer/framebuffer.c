// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// Monochrome (1-Bit Per Pixel) 8-byte x 8-bit Framebuffer

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> // strncmp
#include <time.h>   // timespec
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "framebuffer.h"

int fb_bits_per_pixel(void) { return 1; }

int fb_horizontal_pixels(void) { return 128; }
int fb_vertical_pixels(void) { return 64; }
int fb_pixels(void) { return fb_horizontal_pixels() * fb_vertical_pixels(); }

int fb_horizontal_bytes(void) { return fb_horizontal_pixels() >> 3; }
int fb_vertical_bytes(void) { return fb_vertical_pixels(); }
int fb_bytes(void) { return fb_horizontal_bytes() * fb_vertical_bytes(); }
size_t fb_size(void) { return (size_t)fb_bytes(); }

static void srand_nsec(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec);
}

static int validate_fixed_info(int file) {
    struct fb_fix_screeninfo finfo;
    if (ioctl(file, FBIOGET_FSCREENINFO, &finfo) < 0) {
        printf("Unable to get fixed framebuffer info\n");
        return -1;
    }

    printf("Line Length: %d\n", finfo.line_length);

    int expect = fb_horizontal_bytes();
    if ((int)finfo.line_length != expect) {
        printf("Line length not %d as expected\n", expect);
        return -2;
    }

    return 0;
}

static int validate_variable_info(int file) {
    struct fb_var_screeninfo vinfo;
    if (ioctl(file, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        printf("Unable to get variable framebuffer information\n");
        return -1;
    }

    printf("Screen Resolution: %d x %d\n", vinfo.xres, vinfo.yres);
    printf("Bits Per Pixel: %d\n", vinfo.bits_per_pixel);
    printf("X Offset: %d\n", vinfo.xoffset);
    printf("Y Offset: %d\n", vinfo.yoffset);

    size_t size = (vinfo.xres * vinfo.yres * vinfo.bits_per_pixel) / 8;
    printf("Map Size: %zu\n", size);

    int expect_xres = fb_horizontal_pixels();
    int expect_yres = fb_vertical_pixels();
    if ((int)vinfo.xres != expect_xres || (int)vinfo.yres != expect_yres) {
        printf("Resolution not %d x %d as expected\n", expect_xres, expect_yres);
        return -2;
    }

    int expect_bpp = fb_bits_per_pixel();
    if ((int)vinfo.bits_per_pixel != expect_bpp) {
        printf("Bits per pixel not %d as expected\n", expect_bpp);
        return -3;
    }

    size_t expect_size = fb_size();
    if (size != expect_size) {
        printf("Map size not %zu as expected\n", expect_size);
        return -4;
    }

    return 0;
}

int fb_open(const char* fbdev, int argc, char* argv[]) {
    int index = 1;
    while (index < argc) {
        const char* arg = argv[index++];
        if (strncmp(arg, "--fbdev=", 8) == 0)
            fbdev = &arg[8];
    }

    int file = open(fbdev, O_RDWR);
    if (file < 0) {
        printf("Unable to open framebuffer device %s\n", fbdev);
        return -1;
    }

    if (validate_fixed_info(file) < 0) {
        close(file);
        return -2;
    }

    if (validate_variable_info(file) < 0) {
        close(file);
        return -3;
    }

    return file;
}

void fb_close(int file) {
    close(file);
}

void fb_unmap(void* fb) {
    munmap(fb, fb_size());
}

void* fb_map(int file) {
    void* fb = mmap(0, fb_size(), PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    if (fb == MAP_FAILED) {
        printf("Unable to map framebuffer device memory\n");
        return NULL;
    }

    srand_nsec();
    return fb;
}

void fb_show(void* fb) {
    int rc = msync(fb, fb_size(), MS_SYNC);
    if (rc)
        printf("show: msync() returned %d\n", rc);
}
