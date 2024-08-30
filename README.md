# PlasmaFastLED

Example PlatformIO project for RP2040 using Arduino framework and FastLED library

## Background

I recently obtained these [Pimoroni](https://shop.pimoroni.com) RP2040 "Plasma" boards that are purpose-built for driving programmable LEDs:

- [Plasma Stick 2040 W](https://shop.pimoroni.com/products/plasma-stick-2040-w?variant=40359072301139)
- [Plasma 2040](https://shop.pimoroni.com/products/plasma-2040?variant=39410354847827)

Prior to this I'd only used ESP32-based boards for programmable LED projects. Since I'd already written code using the Arduino framework and FastLED library for ESP32, I wanted to try using the same development environment for RP2040 (as opposed to using Raspberrry Pi Pico SDK and Pimoroni libraries). This simple project is what I wrote to confirm this was possible.

I am sharing the project and these notes in case it helps anyone else in a similar situation. Familiarity with VS Code, PlatformIO, Arduino framework and FastLED is assumed, but I have documented several things I learnt when working with RP2040-based boards in this environment that were not obvious to me when I first did this.

## Prerequisites for Windows

I used VS Code on Windows 11 with the PlatformIO extension for this project. This requires the following to be installed:

- [Git for Windows](https://gitforwindows.org/)
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE extension for VS Code](https://platformio.org/install/ide?install=vscode)

Before continuing, you must enable long filenames in both Windows and Git to avoid installation issues with Arduino-Pico:

- https://learn.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=registry#enable-long-paths-in-windows-10-version-1607-and-later
- `git config --system core.longpaths true`

See [important steps for Windows users](https://arduino-pico.readthedocs.io/en/latest/platformio.html#important-steps-for-windows-users-before-installing) in the [Arduino-Pico documentation](https://arduino-pico.readthedocs.io/en/latest/) for more details.

## Building example project

The following core and platform enable code for the Arduino framework to be built for RP2040-based boards using PlatformIO:

- [Aduino-Pico (earlephilhower) core](https://github.com/earlephilhower/arduino-pico)
- [Raspberry Pi (maxgerhardt) platform](https://github.com/maxgerhardt/platform-raspberrypi)

This enables use of some Arduino libraries such as FastLED that also have RP2040 support:

- [FastLED](https://fastled.io/)

PlatformIO should automatically download and install the core, platform and library when you first open the project folder in VS Code. 

If this does not happen automatically when you open the project folder then try building the project, as that should also make PlatformIO download and install all required components.

Note that downloading and installing these components will take some time to complete.

## Settings in platformio.ini

### Working environments

Most options are common to both boards mentioned earlier, so they are included in the common `[env]` section shared between all environments.

The working environments `[env:plasma_stick_2040w]` and `[env:env:plasma_2040]` just have the approripate `board` setting for the hardware we are building for i.e. `rpipicow` and `pimoroni_plasma2040`.

### Selecting correct Arduino core

There is more than one Arduino core available for RP2040. This project uses the [Aduino-Pico (earlephilhower) core](https://github.com/earlephilhower/arduino-pico) which can be specified with the `board_build.core` setting e.g.

```
[env:pico]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
framework = arduino
board = pico
board_build.core = earlephilhower
```

In this example project, we are building for `rpipicow` and `pimoroni_plasma2040` boards which are currently only available in Arduino-Pico anyway, so the board_build.core setting is optional. See [Selecting the new core](https://arduino-pico.readthedocs.io/en/latest/platformio.html#selecting-the-new-core) for more details.

### Disabling Clockless M0 fallback in FastLED

At the time of writing, building FastLED for RP2040 can result in assembler errors such as "Error: invalid offset, value too big". To avoid this, `FASTLED_RP2040_CLOCKLESS_M0_FALLBACK` is set to zero in `platformio.ini` to disable the code that generates this error. This fallback method is not usually required on RP2040 anyway. See [FastLED PR 1576](https://github.com/FastLED/FastLED/pull/1576) for more details.

```
build_flags =
    -DFASTLED_RP2040_CLOCKLESS_M0_FALLBACK=0
```

## Modifying sample code for your hardware

Please review the values of the preprocessor definitions at the top of `main.cpp` and adjust accordingly for your board and LEDs before deploying the firmware to your board. The example values work with the [Pimoroni 5m LED wire](https://shop.pimoroni.com/products/5m-flexible-rgb-led-wire-50-rgb-leds-aka-neopixel-ws2812-sk6812?variant=40384556171347) used to test this project.

## Uploading firmware

Uploading firmware to RP2040 works differently compared to ESP32, though once everything is configured correctly the process to upload from PlatformIO in VS Code is the same for both.

### Uploading first build

Your first build will have to be uploaded manually if alternative firmware (e.g. MicroPython) was previously installed on your board.

1. Enter bootloader mode by holding down BOOTSEL button while restarting board (e.g. by connecting USB cable or pressing RESET button if present on your board)
2. You should see USB drive named "RPI-RP2" appear in Windows Explorer
3. Copy `firmware.uf2` from `.pio\build\<environment>` folder in project to RPI-RP2 drive to update firmware
4. Board will reboot automatically after .uf2 file has been copied to it
5. If all goes well you should now see your LEDs light up red, green and blue for one second each then display a moving rainbow pattern

### Uploading subsequent builds

Once you have uploaded the firmware for the first time by copying the .u2f file, you should then be able to upload subsequent builds directly from VS Code/PlatformIO. However to do this you may need to install USB drivers for the board when it is bootloader mode:

1. Enter bootloader mode as before
2. Use [Zadig](https://zadig.akeo.ie/) to install required driver for "RP2 Boot (Interface 1)"
    * Select "RP2 Boot (Interface 1)" from drop-down list in Zadig which by default only lists devices with no drivers
    * Current driver is shown on left which should be "NONE" if you have not previously installed driver
    * Driver to install is shown on right, default selection is "WinUSB" which worked for me with both boards
    * Click on "Install Driver" to start installation - this can take a while
    * Refer to [Zadig on-line help](https://github.com/pbatard/libwdi/wiki/Zadig) for more information
3. Reset or power cycle board and confirm that PlatformIO commands in VS Code such as "Upload" and "Upload and Monitor" now work
