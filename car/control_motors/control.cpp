#include "control.h"

#include <algorithm>
#include <cstdlib>

#include "hardware/pwm.h"

Control::Control()
{
    this->encoder = std::make_shared<Encoder>();
    this->resolutionEncoderA = 20;
    this->resolutionEncoderB = 20;
    this->angle_per_pulse = 2*M_PI/ this->resolutionEncoderA;
}

void Control::init_pin()
{
    const uint motor_pins[] = {MOTOR1_INA, MOTOR1_INB, MOTOR2_INA, MOTOR2_INB};
    for (uint pin : motor_pins) {
        gpio_set_function(pin, GPIO_FUNC_PWM);
        uint slice = pwm_gpio_to_slice_num(pin);
        pwm_set_wrap(slice, MOTOR_PWM_MAX);
        pwm_set_clkdiv(slice, MOTOR_PWM_CLK_DIV);
        pwm_set_enabled(slice, true);
        pwm_set_gpio_level(pin, 0);
    }
    this->stop_all_motors();
   
}

void Control::compute_velocity()
{
    auto dt_us = this->encoder->get_time_between_pulses();
    float dt1 = dt_us[0] / 1000000.0f;
    float dt2 = dt_us[1] / 1000000.0f;
    float velocity1_rad = this->angle_per_pulse / dt1;//rad/s
    float velocity2_rad = this->angle_per_pulse / dt2;//rad/s
    this->velocity1 = velocity1_rad * this->wheel_radius; //cm/s
    this->velocity2 = velocity2_rad * this->wheel_radius; //cm/s
}

std::vector<float> Control::get_velocities()
{
    return std::vector<float>{this->velocity1, this->velocity2};
}

void Control::move_forward(int pwm_value)
{
    this->set_motor_pwm(MOTOR1_INA, pwm_value);
    this->set_motor_pwm(MOTOR1_INB, 0);
    this->set_motor_pwm(MOTOR2_INA, static_cast<int>(pwm_value * MOTOR2_BALANCE));
    this->set_motor_pwm(MOTOR2_INB, 0);
}

void Control::move_backward(int pwm_value)
{
    this->set_motor_pwm(MOTOR1_INA, 0);
    this->set_motor_pwm(MOTOR1_INB, pwm_value);
    this->set_motor_pwm(MOTOR2_INA, 0);
    this->set_motor_pwm(MOTOR2_INB, static_cast<int>(pwm_value * MOTOR2_BALANCE));
}

void Control::drive_from_throttle(int16_t throttle)
{
    int pwm_value = this->throttle_to_pwm(throttle);

    if (pwm_value == 0) {
        this->stop_all_motors();
    } else if (throttle > 0) {
        this->move_forward(pwm_value);
    } else {
        this->move_backward(pwm_value);
    }
}

void Control::set_motor_pwm(uint motor_index, int pwm_value)
{
    if (pwm_value <= 0) {
        pwm_set_gpio_level(motor_index, 0);
        return;
    }

    pwm_value = std::max(MOTOR_PWM_MIN, std::min(MOTOR_PWM_MAX, pwm_value));
    pwm_set_gpio_level(motor_index, static_cast<uint16_t>(pwm_value));
}

void Control::stop_all_motors()
{
    this->set_motor_pwm(MOTOR1_INA, 0);
    this->set_motor_pwm(MOTOR1_INB, 0);
    this->set_motor_pwm(MOTOR2_INA, 0);
    this->set_motor_pwm(MOTOR2_INB, 0);
}

int Control::throttle_to_pwm(int16_t throttle)
{
    int magnitude = std::min(THROTTLE_MAX, std::abs(static_cast<int>(throttle)));
    if (magnitude == 0) {
        return 0;
    }

    return MOTOR_PWM_MIN + (magnitude * (MOTOR_PWM_MAX - MOTOR_PWM_MIN)) / THROTTLE_MAX;
}
