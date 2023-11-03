# SPDX-FileCopyrightText: 2023 Alan Mikhak for The Perseus Group, Inc.
#
# SPDX-License-Identifier: MIT
#
# Windows Makefile for I2C USB HID examples

# usage:
# %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
# nmake /f Makefile.mak clean
# nmake /f Makefile.mak

HIDAPI = C:\src\github.com\libusb\hidapi

CFLAGS = /nologo /Oi /W3 /D "WIN32" /D "_WINDOWS" /D "CONFIG_I2C_HID_IO"
CFLAGS = $(CFLAGS) /EHsc /std:c17 /Iio /Imcp2221a
CFLAGS = $(CFLAGS) /I$(HIDAPI)\hidapi /I$(HIDAPI)\windows

HID_IO = io\i2c_hid_io.obj
HID_IO = $(HID_IO) io\i2c_hid_windows.obj

MCP2221A = mcp2221a\mcp2221a_i2c.obj
MCP2221A = $(MCP2221A) mcp2221a\mcp2221a_hid.obj

HID = $(HID_IO) $(MCP2221A)

SEESAW = adafruit\adafruit_seesaw.obj
SEESAW = $(SEESAW) adafruit\seesaw_gpio.obj
SEESAW = $(SEESAW) adafruit\seesaw_neopixel.obj

SEESAW_ADC = $(SEESAW) adafruit/seesaw_adc.obj

SEESAW_ENCODER = $(SEESAW) adafruit/seesaw_encoder.obj

SEESAW_HID_EXE = adafruit/neorotary_i2c_hid.exe
SEESAW_HID_EXE = $(SEESAW_HID_EXE) adafruit/neorotary1x4_i2c_hid.exe
SEESAW_HID_EXE = $(SEESAW_HID_EXE) adafruit/neokey1x4_i2c_hid.exe
SEESAW_HID_EXE = $(SEESAW_HID_EXE) adafruit/neoslider_i2c_hid.exe

HID_EXE = bme280\bme280_i2c_hid.exe
HID_EXE = $(HID_EXE) emc2101\emc2101_i2c_hid.exe
HID_EXE = $(HID_EXE) tlv493d\tlv493d_i2c_hid.exe

MCP2221A_EXE = mcp2221a\scan_i2c_hid.exe

all: mcp2221a hid seesaw_hid

clean:
	del $(HID)
	del bme280\*.obj bme280\*.lib bme280\*.exp bme280\*.exe
	del emc2101\*.obj emc2101\*.lib emc2101\*.exp emc2101\*.exe
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

adafruit\adafruit_seesaw.obj: adafruit\adafruit_seesaw.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\seesaw_encoder.obj: adafruit\seesaw_encoder.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\seesaw_adc.obj: adafruit\seesaw_adc.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\seesaw_gpio.obj: adafruit\seesaw_gpio.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit\seesaw_neopixel.obj: adafruit\seesaw_neopixel.c
	cl $(CFLAGS) /c $? /Fo:$@

mcp2221a\mcp2221a_scan.obj: mcp2221a\mcp2221a_scan.c
	cl $(CFLAGS) /c $? /Fo:$@

mcp2221a\scan_i2c_hid.exe: $(HID) mcp2221a\mcp2221a_scan.obj
	cl $(CFLAGS) $** /Fe:$@

mcp2221a: $(MCP2221A_EXE)

adafruit\neokey1x4.obj: adafruit\neokey1x4.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit/neokey1x4_i2c_hid.exe: $(HID) $(SEESAW_ADC) adafruit/neokey1x4.obj
	cl $(CFLAGS) $** /Fe:$@

adafruit\neoslider.obj: adafruit\neoslider.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit/neoslider_i2c_hid.exe: $(HID) $(SEESAW_ADC) adafruit/neoslider.obj
	cl $(CFLAGS) $** /Fe:$@

adafruit\neorotary.obj: adafruit\neorotary.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit/neorotary_i2c_hid.exe: $(HID) $(SEESAW_ENCODER) adafruit/neorotary.obj
	cl $(CFLAGS) $** /Fe:$@

adafruit\neorotary1x4.obj: adafruit\neorotary1x4.c
	cl $(CFLAGS) /c $? /Fo:$@

adafruit/neorotary1x4_i2c_hid.exe: $(HID) $(SEESAW_ENCODER) adafruit/neorotary1x4.obj
	cl $(CFLAGS) $** /Fe:$@

seesaw_hid: $(SEESAW_HID_EXE)

bme280/bme280.obj: bme280/bme280.c
	cl $(CFLAGS) /c $? /Fo:$@

bme280/bme280_i2c_hid.exe: $(HID) bme280/bme280.obj
	cl $(CFLAGS) $** /Fe:$@

emc2101/emc2101.obj: emc2101/emc2101.c
	cl $(CFLAGS) /c $? /Fo:$@

emc2101/emc2101_i2c_hid.exe: $(HID) emc2101/emc2101.obj
	cl $(CFLAGS) $** /Fe:$@

tlv493d/tlv493d.obj: tlv493d/tlv493d.c
	cl $(CFLAGS) /c $? /Fo:$@

tlv493d/tlv493d_i2c_hid.exe: $(HID) tlv493d/tlv493d.obj
	cl $(CFLAGS) $** /Fe:$@

hid: $(HID_EXE)
