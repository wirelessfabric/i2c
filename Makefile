# SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
#
# SPDX-License-Identifier: MIT
#
# Linux Makefile for I2C examples

dbg ?= 0

ARCH := $(shell uname -m)

HOST_OS := $(shell uname -s 2>/dev/null | tr "[:upper:]" "[:lower:]")

INC_PATH := -Iio

CFLAGS := -Wall -Wextra -Wpedantic -std=c17
ifeq ($(dbg),1)
	CFLAGS += -g3 -D_DEBUG
else
	CFLAGS += -O3 -DNDEBUG
endif
CFLAGS += $(INC_PATH)

LDFLAGS :=

I2C_INC := io/i2c_io.h
I2C_INC += io/i2c_dev_io.h
I2C_INC += io/i2c_dev_context.h

I2C := io/i2c_dev_io.o

FONT := font/font_8x8_mono.o
FONT += font/font_8x8_rotate.o

FRAMEBUFFER := framebuffer/framebuffer_draw.o
FRAMEBUFFER += framebuffer/framebuffer.o

SEESAW := adafruit/adafruit_seesaw.o
SEESAW += adafruit/seesaw_gpio.o
SEESAW += adafruit/seesaw_neopixel.o

SEESAW_ADC := $(SEESAW) adafruit/seesaw_adc.o

SEESAW_ENCODER := $(SEESAW) adafruit/seesaw_encoder.o

HID := io/i2c_hid_io.o
HID += mcp2221a/mcp2221a_i2c.o
HID += mcp2221a/mcp2221a_hid.o

HID_INC := io/i2c_io.h
HID_INC += io/i2c_hid_io.h
HID_INC += io/i2c_hid_context.h
HID_INC += mcp2221a/mcp2221a_context.h
HID_INC += mcp2221a/mcp2221a_i2c.h
HID_INC += mcp2221a/mcp2221a_hid.h

HID_INC_PATH := -Imcp2221a
ifeq ($(HOST_OS),darwin)
	ifeq "$(ARCH)" "x86_64"
		HIDAPI := /usr/local/Cellar/hidapi/0.14.0
	else
		HIDAPI := /opt/homebrew/Cellar/hidapi/0.14.0
	endif
	HID_INC_PATH += -I$(HIDAPI)/include/hidapi
endif

HID_CFLAGS := $(CFLAGS)
HID_CFLAGS += -DCONFIG_I2C_HID_IO
HID_CFLAGS += $(HID_INC_PATH)

HID_LDFLAGS :=
ifeq ($(HOST_OS),darwin)
	HID_LDFLAGS += -L$(HIDAPI)/lib
	HID_LDFLAGS += -lhidapi
else
	HID_LDFLAGS += -lhidapi-libusb
	ifeq "$(ARCH)" "armv7l"
		HID_LDFLAGS += -L/usr/lib/arm-linux-gnueabihf
	endif
endif
HID_LDFLAGS += $(LDFLAGS)

HID_EXE := bme280/bme280_i2c_hid
HID_EXE += bme68x/bme68x_i2c_hid
HID_EXE += emc2101/emc2101_i2c_hid
HID_EXE += tlv493d/tlv493d_i2c_hid
HID_EXE += vcnl4020/vcnl4020_i2c_hid
HID_EXE += pimoroni/buttonshim_i2c_hid
HID_EXE += adafruit/neokey1x4_i2c_hid
HID_EXE += adafruit/neoslider_i2c_hid
HID_EXE += adafruit/neorotary_i2c_hid
HID_EXE += adafruit/neorotary1x4_i2c_hid
HID_EXE += mcp2221a/scan_i2c_hid

I2C_EXE := bme280/bme280
I2C_EXE += bme68x/bme68x
I2C_EXE += emc2101/emc2101
I2C_EXE += lc709203f/lc709203f
I2C_EXE += tlv493d/tlv493d
I2C_EXE += vcnl4020/vcnl4020
I2C_EXE += adafruit/neokey1x4
I2C_EXE += adafruit/neoslider
I2C_EXE += adafruit/neorotary
I2C_EXE += adafruit/neorotary1x4
I2C_EXE += pimoroni/buttonshim
I2C_EXE += pisugar3/pisugar3

FRAMEBUFFER_EXE := ssd1306/ssd1306

EXE := $(FRAMEBUFFER_EXE)
EXE += $(I2C_EXE)
EXE += $(HID_EXE)

all: framebuffer i2c hid

framebuffer: $(FRAMEBUFFER_EXE)

i2c: $(I2C_EXE)

hid: $(HID_EXE)

adafruit/neokey1x4: adafruit/neokey1x4.c $(I2C) $(I2C_INC) $(SEESAW_ADC)
	$(CC) $(CFLAGS) $< $(I2C) $(SEESAW_ADC) $(LDFLAGS) -o $@

adafruit/neoslider: adafruit/neoslider.c $(I2C) $(I2C_INC) $(SEESAW_ADC)
	$(CC) $(CFLAGS) $< $(I2C) $(SEESAW_ADC) $(LDFLAGS) -o $@

adafruit/neorotary: adafruit/neorotary.c $(I2C) $(I2C_INC) $(SEESAW_ENCODER)
	$(CC) $(CFLAGS) $< $(I2C) $(SEESAW_ENCODER) $(LDFLAGS) -o $@

adafruit/neorotary1x4: adafruit/neorotary1x4.c $(I2C) $(I2C_INC) $(SEESAW_ENCODER)
	$(CC) $(CFLAGS) $< $(I2C) $(SEESAW_ENCODER) $(LDFLAGS) -o $@

bme280/bme280: bme280/bme280.c bme280/bme280.h $(I2C) $(I2C_INC)
	$(CC) $(CFLAGS) $< $(I2C) -lm $(LDFLAGS) -o $@

bme68x/bme68x: bme68x/bme68x.c bme68x/bme68x.h $(I2C) $(I2C_INC)
	$(CC) $(CFLAGS) $< $(I2C) -lm $(LDFLAGS) -o $@

emc2101/emc2101: emc2101/emc2101.c emc2101/emc2101.h $(I2C) $(I2C_INC)
	$(CC) $(CFLAGS) $< $(I2C) $(LDFLAGS) -o $@

lc709203f/lc709203f: lc709203f/lc709203f.c lc709203f/lc709203f.h $(I2C) $(I2C_INC)
	$(CC) $(CFLAGS) $< $(I2C) $(LDFLAGS) -o $@

vcnl4020/vcnl4020: vcnl4020/vcnl4020.c vcnl4020/vcnl4020.h $(I2C) $(I2C_INC)
	$(CC) $(CFLAGS) $< $(I2C) $(LDFLAGS) -o $@

pimoroni/buttonshim: pimoroni/buttonshim.c pimoroni/buttonshim.h $(I2C) $(I2C_INC)
	$(CC) $(CFLAGS) $< $(I2C) $(LDFLAGS) -o $@

pisugar3/pisugar3: pisugar3/pisugar3.c pisugar3/pisugar3.h $(I2C) $(I2C_INC)
	$(CC) $(CFLAGS) $< $(I2C) $(LDFLAGS) -o $@

ssd1306/ssd1306: ssd1306/ssd1306.c $(FONT) $(FRAMEBUFFER)
	$(CC) $(CFLAGS) -Iframebuffer -Ifont $< $(FRAMEBUFFER) $(FONT) -lm $(LDFLAGS) -o $@

tlv493d/tlv493d: tlv493d/tlv493d.c tlv493d/tlv493d.h $(I2C) $(I2C_INC)
	$(CC) $(CFLAGS) $< $(I2C) $(LDFLAGS) -o $@

io/i2c_hid_io.o: io/i2c_hid_io.c $(HID_INC)
	$(CC) $(HID_CFLAGS) -c $< -o $@

mcp2221a/mcp2221a_i2c.o: mcp2221a/mcp2221a_i2c.c $(HID_INC)
	$(CC) $(HID_CFLAGS) -c $< -o $@

mcp2221a/mcp2221a_hid.o: mcp2221a/mcp2221a_hid.c $(HID_INC)
	$(CC) $(HID_CFLAGS) -c $< -o $@

mcp2221a/scan_i2c_hid: mcp2221a/mcp2221a_scan.c $(HID) $(HID_INC)
	$(CC) $(HID_CFLAGS) $< $(HID) $(HID_LDFLAGS) -o $@

adafruit/neokey1x4_i2c_hid: adafruit/neokey1x4.c $(HID) $(HID_INC) $(SEESAW_ADC)
	$(CC) $(HID_CFLAGS) $< $(HID) $(SEESAW_ADC) $(HID_LDFLAGS) -o $@

adafruit/neoslider_i2c_hid: adafruit/neoslider.c $(HID) $(HID_INC) $(SEESAW_ADC)
	$(CC) $(HID_CFLAGS) $< $(HID) $(SEESAW_ADC) $(HID_LDFLAGS) -o $@

adafruit/neorotary_i2c_hid: adafruit/neorotary.c $(HID) $(HID_INC) $(SEESAW_ENCODER)
	$(CC) $(HID_CFLAGS) $< $(HID) $(SEESAW_ENCODER) $(HID_LDFLAGS) -o $@

adafruit/neorotary1x4_i2c_hid: adafruit/neorotary1x4.c $(HID) $(HID_INC) $(SEESAW_ENCODER)
	$(CC) $(HID_CFLAGS) $< $(HID) $(SEESAW_ENCODER) $(HID_LDFLAGS) -o $@

bme280/bme280_i2c_hid: bme280/bme280.c bme280/bme280.h $(HID) $(HID_INC)
	$(CC) $(HID_CFLAGS) bme280/bme280.c $(HID) $(HID_LDFLAGS) -lm -o $@

bme68x/bme68x_i2c_hid: bme68x/bme68x.c bme68x/bme68x.h $(HID) $(HID_INC)
	$(CC) $(HID_CFLAGS) bme68x/bme68x.c $(HID) $(HID_LDFLAGS) -lm -o $@

emc2101/emc2101_i2c_hid: emc2101/emc2101.c emc2101/emc2101.h $(HID) $(HID_INC)
	$(CC) $(HID_CFLAGS) emc2101/emc2101.c $(HID) $(HID_LDFLAGS) -o $@

tlv493d/tlv493d_i2c_hid: tlv493d/tlv493d.c tlv493d/tlv493d.h $(HID) $(HID_INC)
	$(CC) $(HID_CFLAGS) tlv493d/tlv493d.c $(HID) $(HID_LDFLAGS) -o $@

vcnl4020/vcnl4020_i2c_hid: vcnl4020/vcnl4020.c vcnl4020/vcnl4020.h $(HID) $(HID_INC)
	$(CC) $(HID_CFLAGS) vcnl4020/vcnl4020.c $(HID) $(HID_LDFLAGS) -o $@

pimoroni/buttonshim_i2c_hid: pimoroni/buttonshim.c pimoroni/buttonshim.h $(HID) $(HID_INC)
	$(CC) $(HID_CFLAGS) tlv493d/tlv493d.c $(HID) $(HID_LDFLAGS) -o $@

clean:
	rm -rf adafruit/*.o adafruit/*.dSYM
	rm -rf adafruit/*.o adafruit/*.dSYM
	rm -rf emc2101/*.o emc2101/*.dSYM
	rm -rf pimoroni/*.o pimoroni/*.dSYM
	rm -rf tlv493d/*.o tlv493d/*.dSYM
	rm -rf vcnl4020/*.o vcnl4020/*.dSYM
	rm -rf font/*.o font/*.dSYM
	rm -rf framebuffer/*.o,framebuffer/*.dSYM
	rm -rf mcp2221a/*.o mcp2221a/*.dSYM
	rm -rf io/*.o io/*.dSYM
	rm -rf $(FRAMEBUFFER_EXE)
	rm -rf $(I2C_EXE)
	rm -rf $(HID_EXE)

ssd1306: ssd1306/ssd1306

lc709203f: lc709203f/lc709203f

pisugar3: pisugar3/pisugar3

mcp2221a: mcp2221a/scan_i2c_hid

bme280: bme280/bme280 bme280/bme280_i2c_hid

bme68x: bme68x/bme68x bme68x/bme68x_i2c_hid

emc2101: emc2101/emc2101 emc2101/emc2101_i2c_hid

buttonshim: pimoroni/buttonshim pimoroni/buttonshim_i2c_hid

tlv493d: tlv493d/tlv493d tlv493d/tlv493d_i2c_hid

vcnl4020: vcnl4020/vcnl4020 vcnl4020/vcnl4020_i2c_hid

neokey1x4: adafruit/neokey1x4 adafruit/neokey1x4_i2c_hid

neoslider: adafruit/neoslider adafruit/neoslider_i2c_hid

neorotary: adafruit/neorotary adafruit/neorotary_i2c_hid

neorotary1x4: adafruit/neorotary1x4 adafruit/neorotary1x4_i2c_hid

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
