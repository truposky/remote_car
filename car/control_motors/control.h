#pragma once
#include <memory>
#include <math.h>
#include <vector>

#include "pico/stdlib.h"
#include "pinOut.h"
#include "encoder/encoder.h"
constexpr int MOTOR_PWM_MAX = 255;
constexpr int MOTOR_PWM_MIN = 90;
constexpr float MOTOR_PWM_CLK_DIV = 24.0f; // About 20 kHz with a 125 MHz sys clock and 8-bit wrap.
constexpr int THROTTLE_MAX = 1000;
constexpr float MOTOR2_BALANCE = 0.85f; // izquierdo va más rápido, reducir hasta igualar

class Control {
    public:
        Control();
        ~Control() = default;
        void init_pin();
        void compute_velocity();
        std::vector<float> get_velocities();
        void move_forward(int pwm_value);
        void move_backward(int pwm_value);
        void drive_from_throttle(int16_t throttle);
        void set_motor_pwm(uint motor_index, int pwm_value);
        void stop_all_motors();

    private:
        int throttle_to_pwm(int16_t throttle);
        std::shared_ptr<Encoder> encoder;
        float angle_per_pulse;
        float resolutionEncoderA = 20;
        float resolutionEncoderB = 20;
        float velocity1 = 0.0f;
        float velocity2 = 0.0f;
        float wheel_radius = 2.3f; //2.3cm radius of the wheel

};
