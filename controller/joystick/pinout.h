#pragma once

#include <stdint.h>

namespace controller {

// GPIO assignments for the analog joystick (KY-023 and similar)
constexpr uint32_t VRX = 27;   // X axis
constexpr uint32_t VRY = 26;   // Y axis
constexpr uint32_t SW  = 22;   // push button (active-low, pull-up)

constexpr uint32_t adc_channel_from_gpio(uint32_t gpio) {
    return gpio - 26;
}

static_assert(VRX >= 26 && VRX <= 28, "VRX must be GPIO26, GPIO27, or GPIO28");
static_assert(VRY >= 26 && VRY <= 28, "VRY must be GPIO26, GPIO27, or GPIO28");

// RP2040 ADC channels: GPIO26=ADC0, GPIO27=ADC1, GPIO28=ADC2.
constexpr uint32_t VRX_ADC = adc_channel_from_gpio(VRX);
constexpr uint32_t VRY_ADC = adc_channel_from_gpio(VRY);

}
