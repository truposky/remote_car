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
// libraries 
#include "hardware/adc.h"
//common
#include "livePulse.h"
#include "protocol.h"
#include "communication.h"
#include "app.h"
//controller
#include "pinOut.h"
#include "joystick.h"

SPI spi;
RF24 radio(NRF24_CE, NRF24_CSN);
LivePulse live_pulse;
bool setup () {
    // Wait briefly for serial debug, but still boot when running from battery.
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
    controller::joystick_init();

    return true;
}

void loop() {
    auto read = controller::joystick_read();

    RC_Command cmd;
    cmd.steering = read.x;
    cmd.throttle = read.y;
    cmd.buttons  = read.pressed ? 0x01 : 0x00;   // bit 0 = turbo

    printf("steer=%d throttle=%d btn=%u\n", cmd.steering, cmd.throttle, cmd.buttons);
    if (!nrf24_communication::send_command(radio, cmd)) {
        printf("problem sending command\n");
    }
    live_pulse.change_live_pulse();
    sleep_ms(50);
}
// Resume this session with:───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
// claude --resume f00b870c-77cc-48cd-99bb-686cf972c523  
