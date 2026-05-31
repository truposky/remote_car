#pragma once

#include <stdint.h>

namespace controller {

// GPIO assignments for the analog joystick (KY-023 and similar)
constexpr uint32_t VRX = 26;   // X axis  -> ADC0
constexpr uint32_t VRY = 27;   // Y axis  -> ADC1
constexpr uint32_t SW  = 22;   // push button (active-low, pull-up)

// ADC channel numbers that correspond to the GPIOs above
constexpr uint32_t VRX_ADC = 0;
constexpr uint32_t VRY_ADC = 1;

}
