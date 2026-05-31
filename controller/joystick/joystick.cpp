#include "joystick.h"
#include "pinout.h"

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

namespace controller {

// ADC on the RP2040 is 12-bit: 0..4095. Idle (center) sits around 2048.
constexpr uint16_t ADC_CENTER    = 2048;
constexpr int16_t  OUTPUT_RANGE  = 1000;   // we map to -1000..+1000
constexpr int16_t  DEAD_ZONE     = 50;     // anything closer to 0 is clamped to 0

static int16_t normalize(uint16_t raw)
{
    int32_t centered = static_cast<int32_t>(raw) - ADC_CENTER;
    int32_t scaled   = (centered * OUTPUT_RANGE) / ADC_CENTER;

    if (scaled >  OUTPUT_RANGE) scaled =  OUTPUT_RANGE;
    if (scaled < -OUTPUT_RANGE) scaled = -OUTPUT_RANGE;

    if (scaled > -DEAD_ZONE && scaled < DEAD_ZONE) {
        scaled = 0;
    }
    return static_cast<int16_t>(scaled);
}

void joystick_init()
{
    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);

    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
}

JoystickReading joystick_read()
{
    adc_select_input(VRX_ADC);
    uint16_t raw_x = adc_read();

    adc_select_input(VRY_ADC);
    uint16_t raw_y = adc_read();

    JoystickReading r;
    r.x = normalize(raw_x);
    r.y = normalize(raw_y);
    r.pressed = (gpio_get(SW) == 0);   // active-low with pull-up
    return r;
}

}
