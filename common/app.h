#pragma once
#include "pico/stdlib.h"

bool setup();
void loop();

int main() {
    stdio_init_all();
    while (!setup()) {
        sleep_ms(1000);
    }

    while (true) {
        loop();
    }

    return 0;
}