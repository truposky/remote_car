#pragma once

class pid {
    public:
        pid(float kp, float ki, float kd);
        ~pid() = default;
        float calculate(float setpoint, float measured_value, float dt);

    private:
        float kp;
        float ki;
        float kd;
        float previous_error = 0.0f;
        float integral = 0.0f;
};