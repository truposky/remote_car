#pragma once

#include "pico/stdlib.h"
#include <cstdint>

namespace controller {

class Joystick {
public:
    Joystick(uint gpio, uint adc_channel);
    void init();
    int16_t read();
    uint16_t get_raw() const { return raw_; }
    uint16_t get_center() const { return center_; }

private:
    static constexpr uint16_t ADC_MAX = 4095;
    static constexpr int16_t OUTPUT_RANGE = 1000;
    static constexpr int16_t DEAD_ZONE = 50;

    int16_t normalize(uint16_t raw);

    uint gpio_;
    uint adc_channel_;
    uint16_t center_;
    uint16_t raw_;
};

}
