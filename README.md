# Remote Control Pi

C++ firmware for an RC car controlled by two Raspberry Pi Pico boards. The system uses two separate firmware targets:

- `controller`: handheld transmitter that reads a joystick and sends radio commands.
- `car`: vehicle receiver that reads commands and drives the motor, servo, and encoder logic.

Wireless communication uses nRF24L01+ modules through the RF24 library.

## Project Structure

```text
.
├── CMakeLists.txt
├── car/                         # Car firmware
├── controller/                  # Controller firmware
├── common/                      # Shared code
│   ├── NRF24_communication/     # RF24 protocol and communication code
│   └── live_pulse/              # Shared utility code
├── Lib/RF24                     # RF24 library submodule
└── pico_sdk_import.cmake        # Pico SDK import helper
```

## Requirements

Install the following tools:

- Raspberry Pi Pico SDK `2.2.0`
- ARM GNU toolchain, including `arm-none-eabi-gcc`
- CMake `3.13` or newer
- Ninja
- Picotool, optional but recommended for flashing from the terminal
- Git with submodule support

If you use the official Raspberry Pi Pico VS Code extension, the project expects the tools under `~/.pico-sdk/`, for example:

```text
~/.pico-sdk/sdk/2.2.0
~/.pico-sdk/toolchain/14_2_Rel1
~/.pico-sdk/cmake/v3.31.5/bin/cmake
~/.pico-sdk/ninja/v1.12.1/ninja
```

You can also use your own installation paths if `PICO_SDK_PATH` is configured.

## Clone

Clone the repository with submodules:

```bash
git clone --recurse-submodules https://github.com/truposky/remote_control_pi.git
cd remote_control_pi
```

If you already cloned it without submodules:

```bash
git submodule update --init --recursive
```

## Configure

Using the Pico SDK tools installed by the VS Code extension:

```bash
~/.pico-sdk/cmake/v3.31.5/bin/cmake -B build -G Ninja
```

Or using tools available in your `PATH`:

```bash
cmake -B build -G Ninja
```

If CMake cannot find the Pico SDK, export `PICO_SDK_PATH` before configuring:

```bash
export PICO_SDK_PATH="$HOME/.pico-sdk/sdk/2.2.0"
cmake -B build -G Ninja
```

## Build

Build both firmware targets:

```bash
ninja -C build
```

Build only the controller firmware:

```bash
ninja -C build controller
```

Build only the car firmware:

```bash
ninja -C build car
```

Generated firmware files are written to:

```text
build/controller/controller.uf2
build/car/car.uf2
```

## Flash

Connect the Raspberry Pi Pico in `BOOTSEL` mode and copy the matching `.uf2` file to the mounted drive.

You can also flash with `picotool`:

```bash
picotool load build/controller/controller.uf2 -fx
picotool load build/car/car.uf2 -fx
```

## Pin Configuration

Pin assignments are defined in:

- `controller/pinOut.h`
- `controller/joystick/pinout.h`
- `car/pinOut.h`

RF24 communication uses SPI0. Check these files before wiring the modules.

## CMake Targets

The project defines two executables:

- `controller`: links `RF24`, `pico_stdlib`, `hardware_spi`, `hardware_gpio`, and `hardware_adc`.
- `car`: links `RF24`, `pico_stdlib`, `hardware_spi`, `hardware_gpio`, and `hardware_pwm`.

USB serial output is enabled for both targets for debugging.
