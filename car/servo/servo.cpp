#include "servo.h"
#include "hardware/pwm.h"

static uint servo_slice;
static uint servo_channel;

// 125 MHz / 125 = 1 µs per count. Wrap at 19999 → 20 ms (50 Hz).
constexpr float SERVO_CLK_DIV = 125.0f;
constexpr uint16_t SERVO_WRAP = 19999;
constexpr uint16_t SERVO_CENTER_US = 1500;
constexpr uint16_t SERVO_RANGE_US = 500;

void servo_init(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    servo_slice = pwm_gpio_to_slice_num(gpio);
    servo_channel = pwm_gpio_to_channel(gpio);
    pwm_set_clkdiv(servo_slice, SERVO_CLK_DIV);
    pwm_set_wrap(servo_slice, SERVO_WRAP);
    pwm_set_chan_level(servo_slice, servo_channel, SERVO_CENTER_US);
    pwm_set_enabled(servo_slice, true);
}

void servo_set_position(int16_t steering) {
    int32_t pulse_us = SERVO_CENTER_US
                     + (static_cast<int32_t>(steering) * SERVO_RANGE_US) / 1000;
    if (pulse_us < SERVO_CENTER_US - SERVO_RANGE_US)
        pulse_us = SERVO_CENTER_US - SERVO_RANGE_US;
    if (pulse_us > SERVO_CENTER_US + SERVO_RANGE_US)
        pulse_us = SERVO_CENTER_US + SERVO_RANGE_US;
    pwm_set_chan_level(servo_slice, servo_channel, static_cast<uint16_t>(pulse_us));
}

void servo_center() {
    pwm_set_chan_level(servo_slice, servo_channel, SERVO_CENTER_US);
}
