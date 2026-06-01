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
//common
#include "protocol.h"
#include "communication.h"
#include "livePulse.h"
#include "app.h"
//car
#include "pinOut.h"
#include "control_motors/control.h"
#include "servo/servo.h"

//global variables
SPI spi;
RF24 radio(NRF24_CE, NRF24_CSN, 1000000);
RC_Command command = {};
LivePulse live_pulse;
absolute_time_t last_packet_time;
absolute_time_t next_radio_retry_time;
Control motor_control;
constexpr int64_t MAXIMUM_TIME_BETWEEN_PACKETS=200 *1000;//us
constexpr int64_t RADIO_RETRY_PERIOD_US = 1000 * 1000;//us
bool radio_ready = false;


bool try_init_radio() {
    printf("radio pins: CE=%u CSN=%u SCK=%u MOSI=%u MISO=%u\n",
           NRF24_CE, NRF24_CSN, SPI_SCK, SPI_MOSI, SPI_MISO);

    if (!radio.begin(&spi)) {
        printf("radio hardware is not responding!!\n");
        printf("isChipConnected=%d\n", radio.isChipConnected());
        radio.printDetails();
        radio_ready = false;
        next_radio_retry_time = make_timeout_time_us(RADIO_RETRY_PERIOD_US);
        return false;
    }

    nrf24_communication::init_radio_rx(radio);
    last_packet_time = get_absolute_time();
    radio_ready = true;
    return true;
}

bool setup() {
    live_pulse.init_live_pulse();
    motor_control.init_pin();
    servo_init(SERVO_PIN);

    // Wait up to 3s for a USB host to open the CDC port, so debug prints
    // aren't lost if we're in the bench. If nothing connects, boot anyway.
    absolute_time_t deadline = make_timeout_time_ms(3000);
    while (!tud_cdc_connected() && !time_reached(deadline)) {
        live_pulse.change_live_pulse();
        sleep_ms(10);
    }

    spi.begin(spi1, SPI_SCK, SPI_MOSI, SPI_MISO);
    try_init_radio();

    return true;
}

void loop() {
    live_pulse.change_live_pulse();

    if (!radio_ready) {
        motor_control.stop_all_motors();
        if (time_reached(next_radio_retry_time)) {
            try_init_radio();
        }
        sleep_ms(50);
        return;
    }

    while (nrf24_communication::receive_command(radio, command)) {
      last_packet_time = get_absolute_time();
    }
    int64_t time_between_packets = absolute_time_diff_us(last_packet_time, get_absolute_time());

    if (time_between_packets > MAXIMUM_TIME_BETWEEN_PACKETS) {
        motor_control.stop_all_motors();
        servo_center();
        printf("[TIMEOUT] no packet for %lld us\n", time_between_packets);
        sleep_ms(50);
        return;
    }

    printf("steer=%d throttle=%d btn=%u\n",
           command.steering, command.throttle, command.buttons);

    servo_set_position(command.steering);
    motor_control.drive_from_throttle(command.throttle);
    sleep_ms(50);
}
