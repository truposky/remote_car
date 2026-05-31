#include "communication.h"

#include <stdio.h>
#include "protocol.h"

void nrf24_communication::init_radio_tx(RF24 &radio) {
    printf("Mode: %s\n", "TX");
    radio.setChannel(RF_CHANNEL);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.setRetries(5, 15);
    radio.enableDynamicPayloads();
    radio.openWritingPipe(RF_PIPE_ADDRESS[0]);
    radio.openReadingPipe(1, RF_PIPE_ADDRESS[1]);
    radio.stopListening();
    printf("Node ready for tx on channel %u\n", RF_CHANNEL);
}

void nrf24_communication::init_radio_rx(RF24 &radio) {
    printf("Mode: %s\n", "RX");
    radio.setChannel(RF_CHANNEL);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.setRetries(5, 15);
    radio.enableDynamicPayloads();
    radio.openReadingPipe(1, RF_PIPE_ADDRESS[0]);
    radio.openWritingPipe(RF_PIPE_ADDRESS[1]);
    radio.startListening();
    printf("Node ready for rx on channel %u\n", RF_CHANNEL);
}