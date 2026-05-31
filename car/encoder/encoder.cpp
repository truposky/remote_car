#include "encoder.h"

void Encoder::init_encoder(uint8_t resolutionA, uint32_t encoder_pinA, uint8_t resolutionB, uint32_t encoder_pinB)
{
    // Initialize encoder A
    this->resolution = resolutionA;
    gpio_init(encoder_pinA);
    gpio_set_dir(encoder_pinA, GPIO_IN);
    gpio_pull_up(encoder_pinA);
    // gpio_init(ENCODER1_B);
    // gpio_set_dir(ENCODER1_B, GPIO_IN);
    // gpio_pull_up(ENCODER1_B);

    // Set up interrupts for encoder A
    gpio_set_irq_enabled_with_callback(encoder_pinA, GPIO_IRQ_EDGE_FALL, true, &Encoder::encoder_isr);
}

std::vector<uint32_t> Encoder::get_encoder_count()
{
    return {encoder1_instance->encoder_count, encoder2_instance->encoder_count};
}

std::vector<uint32_t> Encoder::get_time_between_pulses()
{
    return {encoder1_instance->time_between_pulses, encoder2_instance->time_between_pulses};
}

void Encoder::encoder_isr(uint gpio, uint32_t events)
{
    if (encoder1_instance && gpio == encoder1_instance->encoder_pin) {
        encoder1_instance->handle_pulse();
    }

    if (encoder2_instance && gpio == encoder2_instance->encoder_pin) {
        encoder2_instance->handle_pulse();
    }

}

void Encoder::handle_pulse()
{
    absolute_time_t now = get_absolute_time();

    time_between_pulses = absolute_time_diff_us(last_pulse_time, now);
    last_pulse_time = now;

    encoder_count++;
}
