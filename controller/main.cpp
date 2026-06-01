/**
 * @file main.cpp
 * @author Alex
 * @brief Communication between two RP2040 using nRF24L01+ transceiver modules.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include <RF24.h>
#include <tusb.h>
#include "hardware/adc.h"
//common
#include "livePulse.h"
#include "protocol.h"
#include "communication.h"
#include "app.h"
//controller
#include "pinOut.h"
#include "joystick.h"
#include "pinout.h"

SPI spi;
RF24 radio(NRF24_CE, NRF24_CSN);
LivePulse live_pulse;
controller::Joystick steer_stick(controller::VRX, controller::VRX_ADC);
controller::Joystick throttle_stick(controller::VRY, controller::VRY_ADC);

bool setup () {
    absolute_time_t deadline = make_timeout_time_ms(3000);
    while (!tud_cdc_connected() && !time_reached(deadline)) {
        sleep_ms(10);
    }

    spi.begin(spi0, SPI_SCK, SPI_MOSI, SPI_MISO);
    if (!radio.begin(&spi)) {
        printf("radio hardware is not responding!!\n");
        return false;
    }
    live_pulse.init_live_pulse();
    nrf24_communication::init_radio_tx(radio);

    adc_init();
    steer_stick.init();
    throttle_stick.init();
    printf("JOYSTICK x_gpio=%u x_adc=%u y_gpio=%u y_adc=%u\n",
           controller::VRX, controller::VRX_ADC,
           controller::VRY, controller::VRY_ADC);
    printf("CENTER steer=%u throttle=%u\n",
           steer_stick.get_center(), throttle_stick.get_center());

    gpio_init(controller::SW);
    gpio_set_dir(controller::SW, GPIO_IN);
    gpio_pull_up(controller::SW);

    return true;
}

void loop() {
    int16_t steer_val = steer_stick.read();
    int16_t throttle_val = throttle_stick.read();
    bool pressed = (gpio_get(controller::SW) == 0);

    RC_Command cmd;
    cmd.steering = steer_val;
    cmd.throttle = -throttle_val;
    cmd.buttons  = pressed ? 0x01 : 0x00;

    printf("raw_x=%u raw_y=%u | steer=%d throttle=%d btn=%u\n",
           steer_stick.get_raw(), throttle_stick.get_raw(),
           cmd.steering, cmd.throttle, cmd.buttons);

    if (!nrf24_communication::send_command(radio, cmd)) {
        printf("problem sending command\n");
    }
    live_pulse.change_live_pulse();
    sleep_ms(50);
}
