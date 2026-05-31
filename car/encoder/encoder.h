#pragma once

#include <vector>


#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

#include "pinOut.h"

class Encoder {
    public:
        Encoder() = default;
        ~Encoder() = default;
        void init_encoder(uint8_t resolutionA, uint32_t encoder_pinA, uint8_t resolutionB, uint32_t encoder_pinB);
        std::vector<uint32_t> get_encoder_count();
        std::vector<uint32_t> get_time_between_pulses();
    private:
        
        static void encoder_isr(uint gpio, uint32_t events);
        static Encoder* encoder1_instance;
        static Encoder* encoder2_instance;
        void handle_pulse();
        uint32_t encoder_pin;
        uint8_t resolution;

        volatile int32_t encoder_count = 0;
        volatile absolute_time_t last_pulse_time;
        volatile uint32_t time_between_pulses = 0;
};