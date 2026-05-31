#include <stdio.h>

#include "pico/stdlib.h"
#include <tusb.h>
#include <RF24.h>

constexpr uint32_t SPI_MISO = 4;
constexpr uint32_t SPI_MOSI = 3;
constexpr uint32_t SPI_SCK = 2;

constexpr uint32_t NRF24_CSN = 5;
constexpr uint32_t NRF24_CE = 17;

// Cambia esta constante en cada Pico:
// true  -> transmisor
// false -> receptor
constexpr bool IS_TX = true;
constexpr bool RADIO_NUMBER = IS_TX ? 0 : 1;
constexpr uint8_t RF_CHANNEL = 108;

SPI spi;
RF24 radio(NRF24_CE, NRF24_CSN);

const uint8_t address[][6] = {"1Node", "2Node"};
float payload = 0.0;

bool setup()
{
    while (!tud_cdc_connected()) {
        sleep_ms(10);
    }

    spi.begin(spi0, SPI_SCK, SPI_MOSI, SPI_MISO);
    if (!radio.begin(&spi)) {
        printf("radio hardware is not responding!!\n");
        return false;
    }

    printf("RF24/examples_pico/gettingStarted\n");
    printf("Modo: %s\n", IS_TX ? "TX" : "RX");
    printf("radioNumber = %d\n", (int)RADIO_NUMBER);

    radio.setChannel(RF_CHANNEL);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.setRetries(5, 15);
    radio.setPayloadSize(sizeof(payload));
    radio.stopListening(address[RADIO_NUMBER]);
    radio.openReadingPipe(1, address[!RADIO_NUMBER]);

    if (IS_TX) {
        radio.stopListening();
        printf("Nodo listo para transmitir en canal %u\n", RF_CHANNEL);
    } else {
        radio.startListening();
        printf("Nodo escuchando paquetes en canal %u\n", RF_CHANNEL);
    }

    return true;
}

void loop()
{
    if (IS_TX) {
        uint64_t start_timer = to_us_since_boot(get_absolute_time());
        bool report = radio.write(&payload, sizeof(payload));
        uint64_t end_timer = to_us_since_boot(get_absolute_time());

        if (report) {
            printf("Transmission successful! Time to transmit = %llu us. Sent: %f\n", end_timer - start_timer, payload);
            payload += 0.01;
        } else {
            printf("Transmission failed or timed out (sin ACK del receptor)\n");
        }

        sleep_ms(1000);
    } else {
        uint8_t pipe;
        if (radio.available(&pipe)) {
            uint8_t bytes = radio.getPayloadSize();
            radio.read(&payload, bytes);
            printf("Received %d bytes on pipe %d: %f\n", bytes, pipe, payload);
        }
        sleep_ms(10);
    }
}


int main()
{
    stdio_init_all();

    while (!setup()) {
    }
    while (true) {
        loop();
    }
    return 0;
}
