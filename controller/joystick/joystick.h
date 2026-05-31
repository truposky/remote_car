#pragma once

#include <stdint.h>

namespace controller {

// Reading from the joystick, already normalized.
//   x, y   : -1000 (full negative) .. 0 (center) .. +1000 (full positive)
//   pressed: true if the SW button is currently held down
struct JoystickReading {
    int16_t x;
    int16_t y;
    bool    pressed;
};

// Configure ADC + SW GPIO. Call once from setup().
void joystick_init();

// Take a fresh sample from the joystick and return it already mapped
// to the -1000..+1000 range with a dead-zone around the center.
JoystickReading joystick_read();

}
