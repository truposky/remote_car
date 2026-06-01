#include "joystick.h"
#include "hardware/adc.h"

namespace controller {

Joystick::Joystick(uint gpio, uint adc_channel)
    : gpio_(gpio), adc_channel_(adc_channel), center_(0), raw_(0) {}

void Joystick::init() {
    adc_gpio_init(gpio_);
    adc_select_input(adc_channel_);
    sleep_us(20);
    adc_read();
    adc_read();
    this->center_ = adc_read();
}

int16_t Joystick::read() {
    adc_select_input(adc_channel_);
    sleep_us(20);
    adc_read();
    adc_read();
    this->raw_ = adc_read();
    return normalize(this->raw_);
}

int16_t Joystick::normalize(uint16_t raw) {
    int32_t centered = static_cast<int32_t>(raw) - this->center_;
    int32_t range = (centered >= 0)
                  ? static_cast<int32_t>(ADC_MAX - this->center_)
                  : static_cast<int32_t>(this->center_);
    if (range <= 0) {
        return 0;
    }

    int32_t scaled = (centered * OUTPUT_RANGE) / range;

    if (scaled > OUTPUT_RANGE) {scaled = OUTPUT_RANGE;}
    if (scaled < -OUTPUT_RANGE) {scaled = -OUTPUT_RANGE;}
    if (scaled > -DEAD_ZONE && scaled < DEAD_ZONE) {scaled = 0;}

    return static_cast<int16_t>(scaled);
}

}
