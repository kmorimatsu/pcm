cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(lcd-lib C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()


add_executable(pcm_echo
        main.c
        )

target_link_libraries(pcm_echo pico_stdlib)
target_link_libraries(pcm_echo hardware_pwm)
target_link_libraries(pcm_echo hardware_adc)

pico_add_extra_outputs(pcm_echo)

