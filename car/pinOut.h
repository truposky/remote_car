#pragma once

#include "pico/stdlib.h"

// nRF24L01+
constexpr uint32_t SPI_MISO = 12;
constexpr uint32_t NRF24_CSN = 15;
constexpr uint32_t SPI_SCK = 14;
constexpr uint32_t SPI_MOSI = 11;
constexpr uint32_t NRF24_CE = 13;

//Motor A
constexpr uint MOTOR1_INA = 2;
constexpr uint MOTOR1_INB = 3;
//Motor B
constexpr uint MOTOR2_INA = 4;
constexpr uint MOTOR2_INB = 5;
//Encoder A
constexpr uint ENCODER1_A = 10;
constexpr uint ENCODER1_B = 6;
