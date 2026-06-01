#pragma once

#include "pico/stdlib.h"
#include <cstdint>

void servo_init(uint gpio);
void servo_set_position(int16_t steering);
void servo_center();
