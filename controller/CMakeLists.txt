set(CONTROLLER_SRC
    main.cpp
    joystick/joystick.cpp
    ${CMAKE_SOURCE_DIR}/common/live_pulse/livePulse.cpp
    ${CMAKE_SOURCE_DIR}/common/NRF24_communication/communication.cpp
)

add_executable(controller ${CONTROLLER_SRC})

target_include_directories(controller PRIVATE
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/common/NRF24_communication
    ${CMAKE_SOURCE_DIR}/common/live_pulse
    ${CMAKE_CURRENT_LIST_DIR}/joystick
)

target_link_libraries(controller
    RF24
    pico_stdlib
    hardware_spi
    hardware_gpio
    hardware_adc
)

pico_enable_stdio_usb(controller 1)
pico_enable_stdio_uart(controller 0)
pico_add_extra_outputs(controller)
