// SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
//
// SPDX-License-Identifier: MIT
//
// MCP2221A - Microchip USB to I2C Host Protocol Bridge
// Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/20005565B.pdf
//
// http://ww1.microchip.com/downloads/en/DeviceDoc/mcp2221_0_1.tar.gz
// https://github.com/raspberrypi/linux/blob/rpi-6.1.y/drivers/hid/hid-mcp2221.c
//
// https://github.com/libusb/hidapi.git
//

#include <stdio.h>
#include <stdlib.h> // strtoul
#include "mcp2221a_hid.h"

// # of times to retry opening after reset
#define HID_REOPEN_RETRIES 50

// # of microseconds between reopen attempts after reset
#define HID_REOPEN_DELAY 15000 

static inline void mcp2221a_hid_sleep(unsigned long usec) {
#if defined(_WIN32) || defined(_WIN64)
    Sleep((DWORD)(usec / 1000));
#else
    usleep(usec);
#endif
}

static inline bool mcp2221a_hid_debug(context* context) {
    return context->hid.debug;
}

static inline void mcp2221a_hid_transfer_delay(context* context) {
    if (context->hid.delay_usec)
        mcp2221a_hid_sleep(context->hid.delay_usec);
}

static inline void mcp2221a_hid_reopen_delay(context* context) {
    if (context->hid.reopen_delay_usec)
        mcp2221a_hid_sleep(context->hid.reopen_delay_usec);
}

int mcp2221a_hid_init(context* context, int argc, char* argv[]) {
    context->hid.reopen_delay_usec = HID_REOPEN_DELAY;

    int index = 1;
    while (index < argc) {
        const char* arg = argv[index++];
        if (strcmp(arg, "--dbg") == 0)
            context->hid.debug = true;
        else if (strncmp(arg, "--hid_serial_number=", 20) == 0)
            context->hid.serial_number = &arg[20];
        else if (strncmp(arg, "--hid_delay=", 12) == 0)
            context->hid.delay_usec = strtoul(&arg[12], NULL, 0);
        else if (strncmp(arg, "--hid_reopen_delay=", 19) == 0)
            context->hid.reopen_delay_usec = strtoul(&arg[19], NULL, 0);
    }

    int result = hid_init();
    if (result < 0 && mcp2221a_hid_debug(context))
        printf("hid_init() failed %d\n", result);
    return result;
}

void mcp2221a_hid_exit(context* context) {
    mcp2221a_hid_close(context);
    int result = hid_exit();
    if (result < 0 && mcp2221a_hid_debug(context))
        printf("hid_exit() failed %d\n", result);
}

int mcp2221a_hid_write(context* context, uint8_t* buffer) {
    buffer[HID_REPORT_ID] = MCP2221A_HID_REPORT_ID;
    int result = hid_write(context->hid.dev, buffer, MCP2221A_WRITE_SIZE);
    if (result == MCP2221A_WRITE_SIZE)
        return MCP2221A_SUCCESS;

    if (mcp2221a_hid_debug(context))
        printf("hid_write() failed %d\n", result);
    return MCP2221A_WRITE_ERROR;
}

int mcp2221a_hid_read(context* context, uint8_t* buffer) {
    int result = hid_read(context->hid.dev, buffer, MCP2221A_READ_SIZE);
    if (result == MCP2221A_READ_SIZE)
        return MCP2221A_SUCCESS;

    if (mcp2221a_hid_debug(context))
        printf("hid_read() failed %d\n", result);
    return MCP2221A_READ_ERROR;
}

int mcp2221a_hid_transfer(context* context, uint8_t* buffer) {
    if (mcp2221a_hid_write(context, buffer) < 0) {
        if (mcp2221a_hid_debug(context))
            printf("hid_transfer: write failed\n");
        return MCP2221A_WRITE_ERROR;
    }

    mcp2221a_hid_transfer_delay(context);

    if (mcp2221a_hid_read(context, buffer) < 0) {
        if (mcp2221a_hid_debug(context))
            printf("hid_transfer: read failed\n");
        return MCP2221A_READ_ERROR;
    }

    return MCP2221A_SUCCESS;
}

int mcp2221a_hid_reset(context* context) {
    uint8_t buffer[MCP2221A_WRITE_SIZE];
    memset(buffer, 0, sizeof(buffer));
    buffer[MCP2221A_COMMAND] = MCP2221A_RESET_CHIP_COMMAND;
    buffer[MCP2221A_COMMAND + 1] = 0xAB;
    buffer[MCP2221A_COMMAND + 2] = 0xCD;
    buffer[MCP2221A_COMMAND + 3] = 0xEF;
    return mcp2221a_hid_write(context, buffer);
}

static hid_device* mcp2221a_hid_enumerate(context* context, unsigned short vid, unsigned short pid) {
    wchar_t* wstr = context->hid.wstr;
    wchar_t serial_number = 0x0000;
    if (context->hid.serial_number) {
        serial_number = (wchar_t)strtoul(context->hid.serial_number, NULL, 0);
    }

	struct hid_device_info* list = hid_enumerate(vid, pid);
	struct hid_device_info* info = list;
    hid_device* dev = NULL;

	while (info) {
		if (info->vendor_id == vid && info->product_id == pid) {
            dev = hid_open_path(info->path);
            if (dev) {
                if (!serial_number)
                    break;

                if (info->serial_number && info->serial_number[0] == serial_number)
                    break;

                wstr[0] = 0x0000;
                if (hid_get_serial_number_string(dev, wstr, MAX_STR) == 0 && serial_number == wstr[0])
                    break;

                hid_close(dev);
                dev = NULL;
            }
		}
		info = info->next;
	}

	if (list)
        hid_free_enumeration(list);
    
    return dev;
}

int mcp2221a_hid_open(context* context) {
    context->hid.dev = mcp2221a_hid_enumerate(context, MCP2221A_USB_VID, MCP2221A_USB_PID);
    if (!context->hid.dev) {
        if (mcp2221a_hid_debug(context))
            printf("hid_open() failed\n");
        return MCP2221A_OPEN_ERROR;
    }

    int result = mcp2221a_hid_reset(context);
    mcp2221a_hid_close(context);
    if (result < 0) {
        if (mcp2221a_hid_debug(context))
            printf("hid_open: reset failed\n");
        return MCP2221A_RESET_ERROR;
    }

    wchar_t* wstr = context->hid.wstr;
    for (int reopen=0; reopen < HID_REOPEN_RETRIES; reopen++) {
        mcp2221a_hid_reopen_delay(context);

        context->hid.dev = mcp2221a_hid_enumerate(context, MCP2221A_USB_VID, MCP2221A_USB_PID);
        if (!context->hid.dev)
            continue;

        wstr[0] = 0x0000;
        if (hid_get_manufacturer_string(context->hid.dev, wstr, MAX_STR) == 0)
            return MCP2221A_SUCCESS;

        mcp2221a_hid_close(context);
    }

    if (mcp2221a_hid_debug(context))
        printf("hid_open: reopen failed\n");
    return MCP2221A_REOPEN_ERROR;
}

void mcp2221a_hid_close(context* context) {
    if (context->hid.dev) {
        hid_close(context->hid.dev);
        context->hid.dev = NULL;
    }
}

void mcp2221a_hid_show_info(context* context) {
    if (!context->hid.dev)
        return;
    
    hid_device* dev = context->hid.dev;
    wchar_t* wstr = context->hid.wstr;

    wstr[0] = 0x0000;
    if (hid_get_manufacturer_string(dev, wstr, MAX_STR) == 0)
        printf("Manufacturer String: %ls\n", wstr);

    wstr[0] = 0x0000;
    if (hid_get_product_string(dev, wstr, MAX_STR) == 0)
        printf("Product String: %ls\n", wstr);

    wstr[0] = 0x0000;
    if (hid_get_serial_number_string(dev, wstr, MAX_STR) == 0 && wstr[0])
        printf("Serial Number: %d\n", wstr[0]);

    wstr[0] = 0x0000;
    if (hid_get_indexed_string(dev, 1, wstr, MAX_STR) == 0)
        printf("Indexed String 1: %ls\n", wstr);
}
