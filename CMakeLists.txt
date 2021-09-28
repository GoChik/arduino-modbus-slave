cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_TOOLCHAIN_FILE arduino/Arduino-toolchain.cmake)
set(ARDUINO_BOARD_OPTIONS_FILE BoardOptions.cmake)

project(modbus-slave CXX)

add_executable(modbus_slave main.cpp)
include_directories(./)

# Arduino.h is included in hello_world.cpp, so link with Arduino core
target_link_arduino_libraries(modbus_slave PRIVATE core RS485 Modbus)
target_enable_arduino_upload(modbus_slave)
