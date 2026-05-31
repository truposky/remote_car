set(CAR_SRC
    main.cpp
    control_motors/control.cpp
    encoder/encoder.cpp
    ${CMAKE_SOURCE_DIR}/common/NRF24_communication/communication.cpp
    ${CMAKE_SOURCE_DIR}/common/live_pulse/livePulse.cpp
)

add_executable(car ${CAR_SRC})

target_include_directories(car PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/common/NRF24_communication
    ${CMAKE_SOURCE_DIR}/common/live_pulse
    )

target_link_libraries(car
    RF24
    pico_stdlib
    hardware_spi
    hardware_gpio
    hardware_pwm
)

pico_enable_stdio_usb(car 1)
pico_enable_stdio_uart(car 0)
pico_add_extra_outputs(car)
