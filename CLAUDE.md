# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Raspberry Pi Pico (RP2040) embedded C++ project for an RC car remote control system. Two Pico boards communicate wirelessly via nRF24L01+ (2.4GHz). One runs the `controller` firmware (handheld transmitter with joystick), the other runs the `car` firmware (receiver driving motor + servo).

## Build & Flash

**Build** (requires CMake configure first if no `build/` directory):
```bash
# Initial configure (done once, or if CMakeLists.txt changes)
~/.pico-sdk/cmake/v3.31.5/bin/cmake -B build -G Ninja

# Compile both targets
~/.pico-sdk/ninja/v1.12.1/ninja -C build/
```

**Flash** (Pico must be in BOOTSEL mode or connected via picotool):
```bash
picotool load build/controller/controller.uf2 -fx   # mando
picotool load build/car/car.uf2 -fx                 # coche
```

**VS Code tasks** (`.vscode/tasks.json`) wrap these commands as "Compile Project" and "Flash Project".

## Architecture

### Project structure

```
shared/protocol.h       # RCCommand struct + RF24 constants shared by both firmwares
controller/main.cpp     # Mando: reads joystick ADC, transmits RCCommand
car/main.cpp            # Coche: receives RCCommand, drives motor + servo via PWM
Lib/RF24/               # Vendored nRF24L01 library (do not modify)
main.cpp                # Original prototype (unused, safe to delete)
```

### Shared protocol (`shared/protocol.h`)

Both firmwares must include this header. It defines the RF24 channel, pipe addresses, and the `RCCommand` payload struct so both sides always stay in sync.

### controller firmware

- Links: `RF24`, `pico_stdlib`, `hardware_spi`, `hardware_gpio`, `hardware_adc`
- USB serial enabled (useful for debug)
- Reads joystick via ADC (GPIO 26/27/28), packs into `RCCommand`, transmits at ~50 Hz

### car firmware

- Links: `RF24`, `pico_stdlib`, `hardware_spi`, `hardware_gpio`, `hardware_pwm`
- USB serial disabled (runs standalone)
- Receives `RCCommand`, outputs PWM for servo (steering) and motor driver (throttle)
- Must implement a signal-loss watchdog: if no packet received within ~200 ms, stop motors

### Hardware pin assignments (SPI0, both boards)

| Signal | GPIO |
|--------|------|
| SCK    | 2    |
| MOSI   | 3    |
| MISO   | 4    |
| CSN    | 5    |
| CE     | 17   |

### RF24 library (`Lib/RF24/`)

Vendored dependency. Pico-specific SPI/GPIO drivers live in `Lib/RF24/utility/rp2/`. Built as a CMake `INTERFACE` library and linked into both targets via the root `CMakeLists.txt`.

### CMake toolchain

Relies on `~/.pico-sdk/` (SDK 2.2.0, arm-none-eabi-gcc 14.2, CMake 3.31.5, Ninja 1.12.1) as configured in `.vscode/settings.json`.

## Debugging

Via OpenOCD + CMSIS-DAP using the VS Code Cortex-Debug extension (`.vscode/launch.json`). Cortex-M0+ target over SWD. Two profiles: embedded OpenOCD (launches its own server) and external (attaches to a running server).
