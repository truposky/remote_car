#pragma once

#include <RF24.h>
#include "protocol.h"

namespace nrf24_communication {

void init_radio_tx(RF24 &radio);
void init_radio_rx(RF24 &radio);

template <typename T>
bool send_command (RF24 &radio, const T& payload) {
    return radio.write(&payload,sizeof(T));
}

template <typename T>
bool receive_command (RF24 &radio, T& out) {
    if(!radio.available()) {return false;}
    radio.read(&out, sizeof(T));
    return true;
}

}
