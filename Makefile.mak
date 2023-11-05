# SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
#
# SPDX-License-Identifier: MIT
#
# Windows Makefile for I2C USB HID examples

# usage:
# %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
# nmake.exe /f Makefile.mak clean
# nmake.exe /f Makefile.mak

HIDAPI = ..\..\libusb\hidapi

CFLAGS = /nologo /Oi /W3 /D "WIN32" /D "_WINDOWS" /D "CONFIG_I2C_HID_IO"
CFLAGS = $(CFLAGS) /EHsc /std:c17 /Iio /Imcp2221a
CFLAGS = $(CFLAGS) /I$(HIDAPI)\hidapi /I$(HIDAPI)\windows

HID_IO = io\i2c_hid_io.obj
HID_IO = $(HID_IO) io\i2c_hid_windows.obj

MCP2221A = mcp2221a\mcp2221a_i2c.obj
MCP2221A = $(MCP2221A) mcp2221a\mcp2221a_hid.obj

HID = $(HID_IO) $(MCP2221A)

SEESAW = adafruit\seesaw_i2c_hid.obj
SEESAW = $(SEESAW) adafruit\gpio_i2c_hid.obj
SEESAW = $(SEESAW) adafruit\neopixel_i2c_hid.obj

SEESAW_ADC = $(SEESAW) adafruit/adc_i2c_hid.obj

SEESAW_ENCODER = $(SEESAW) adafruit/encoder_i2c_hid.obj

SEESAW_HID_EXE = adafruit/neorotary_i2c_hid.exe
SEESAW_HID_EXE = $(SEESAW_HID_EXE) adafruit/neorotary1x4_i2c_hid.exe
SEESAW_HID_EXE = $(SEESAW_HID_EXE) adafruit/neokey1x4_i2c_hid.exe
SEESAW_HID_EXE = $(SEESAW_HID_EXE) adafruit/neoslider_i2c_hid.exe

HID_EXE = bme280\bme280_i2c_hid.exe
HID_EXE = $(HID_EXE) emc2101\emc2101_i2c_hid.exe
HID_EXE = $(HID_EXE) pimoroni\buttonshim_i2c_hid.exe
HID_EXE = $(HID_EXE) tlv493d\tlv493d_i2c_hid.exe

MCP2221A_EXE = mcp2221a\scan_i2c_hid.exe

all: mcp2221a hid seesaw_hid

clean:
	del $(HID)
	del bme280\*.obj bme280\*.lib bme280\*.exp bme280\*.exe
	del emc2101\*.obj emc2101\*.lib emc2101\*.exp emc2101\*.exe
	del pimoroni\*.obj pimoroni\*.lib pimoroni\*.exp pimoroni\*.exe
	del tlv493d\*.obj tlv493d\*.lib tlv493d\*.exp tlv493d\*.exe
	del mcp2221a\*.obj mcp2221a\*.lib mcp2221a\*.exp mcp2221a\*.exe
	del adafruit\*.obj adafruit\*.lib adafruit\*.exp adafruit\*.exe

io\i2c_hid_windows.obj: $(HIDAPI)\windows\hid.c
	cl $(CFLAGS) /c $? /Fo:$@

io\i2c_hid_io.obj: io\i2c_hid_io.c
	cl $(CFLAGS) /c $? /Fo:$@

mcp2221a\mcp2221a_i2c.obj: mcp2221a\mcp2221a_i2c.c
	cl $(CFLAGS) /c $? /Fo:$@

mcp2221a\mcp2221a_hid.obj: mcp2221a\mcp2221a_hid.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\seesaw_i2c_hid.obj: adafruit\adafruit_seesaw.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\encoder_i2c_hid.obj: adafruit\seesaw_encoder.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\adc_i2c_hid.obj: adafruit\seesaw_adc.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\gpio_i2c_hid.obj: adafruit\seesaw_gpio.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\neopixel_i2c_hid.obj: adafruit\seesaw_neopixel.c
	cl $(CFLAGS) /c $? /Fo:$@

mcp2221a\scan_i2c_hid.obj: mcp2221a\mcp2221a_scan.c
	cl $(CFLAGS) /c $? /Fo:$@

mcp2221a\scan_i2c_hid.exe: $(HID) mcp2221a\scan_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

mcp2221a: $(MCP2221A_EXE)

adafruit\neokey1x4_i2c_hid.obj: adafruit\neokey1x4.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit/neokey1x4_i2c_hid.exe: $(HID) $(SEESAW_ADC) adafruit/neokey1x4_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

adafruit\neoslider_i2c_hid.obj: adafruit\neoslider.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit/neoslider_i2c_hid.exe: $(HID) $(SEESAW_ADC) adafruit/neoslider_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

adafruit\neorotary_i2c_hid.obj: adafruit\neorotary.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit/neorotary_i2c_hid.exe: $(HID) $(SEESAW_ENCODER) adafruit/neorotary_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

adafruit\neorotary1x4_i2c_hid.obj: adafruit\neorotary1x4.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit/neorotary1x4_i2c_hid.exe: $(HID) $(SEESAW_ENCODER) adafruit/neorotary1x4_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

seesaw_hid: $(SEESAW_HID_EXE)

bme280/bme280_i2c_hid.obj: bme280/bme280.c
	cl $(CFLAGS) /c $? /Fo:$@

bme280/bme280_i2c_hid.exe: $(HID) bme280/bme280_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

emc2101/emc2101_i2c_hid.obj: emc2101/emc2101.c
	cl $(CFLAGS) /c $? /Fo:$@

emc2101/emc2101_i2c_hid.exe: $(HID) emc2101/emc2101_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

pimoroni/buttonshim_i2c_hid.obj: pimoroni/buttonshim.c
	cl $(CFLAGS) /c $? /Fo:$@

pimoroni/buttonshim_i2c_hid.exe: $(HID) pimoroni/buttonshim_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

tlv493d/tlv493d_i2c_hid.obj: tlv493d/tlv493d.c
	cl $(CFLAGS) /c $? /Fo:$@

tlv493d/tlv493d_i2c_hid.exe: $(HID) tlv493d/tlv493d_i2c_hid.obj
	cl $(CFLAGS) $** /Fe:$@

hid: $(HID_EXE)
