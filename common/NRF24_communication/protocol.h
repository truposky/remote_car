#pragma once
#include <cstdint>

constexpr uint8_t RF_CHANNEL = 108;
constexpr uint8_t RF_PIPE_ADDRESS[2][5] = {
      { 0xCF, 0xBF, 0xC6, 0x42, 0x63 },
      { 0x1A, 0xA2, 0xE8, 0xA5, 0x5D },
};                                                                                                                                                                                           
             
struct RF24Config {
    uint32_t spi_miso;
    uint32_t spi_mosi;
    uint32_t spi_sck;
    uint32_t nrf24_csn;
    uint32_t nrf24_ce;
    uint8_t rf_channel;
};

struct RC_Command {
    int16_t steering; //x axis
    int16_t throttle; //y axis
    uint8_t buttons; //bitmask for buttons
};
