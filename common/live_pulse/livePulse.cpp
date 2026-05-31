#include "livePulse.h"

void LivePulse::init_live_pulse()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // off
    next_led_pulse_time = get_absolute_time();
    led_off_time = get_absolute_time();
}

bool LivePulse::time_reached_for_next_pulse()
{
    if (led_is_on) {
        return time_reached(led_off_time);
    }

    return time_reached(next_led_pulse_time);
}

bool LivePulse::change_live_pulse()
{
    if(this->time_reached_for_next_pulse()) {
        if(led_is_on) {
            gpio_put(LED_PIN, 0); // off
            led_is_on = false;
        } else {
            gpio_put(LED_PIN, 1); // on
            led_is_on = true;
            this->led_off_time = make_timeout_time_us(LED_ON_TIME_US);
            this->next_led_pulse_time = make_timeout_time_us(LED_PERIOD_US);
        }
        return true;
    }
    return false;
}
   
