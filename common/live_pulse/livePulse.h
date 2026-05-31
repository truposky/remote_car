#pragma once
#include "pico/stdlib.h"
//led
constexpr uint LED_PIN = PICO_DEFAULT_LED_PIN; // GPIO 25
constexpr int64_t LED_ON_TIME_US = 40 * 1000;
constexpr int64_t LED_PERIOD_US = 500 * 1000;

class LivePulse {
    public:
        void init_live_pulse();
        bool time_reached_for_next_pulse();
        bool change_live_pulse();

    private:
        bool led_is_on = false;
        absolute_time_t next_led_pulse_time;
        absolute_time_t led_off_time;
};
