# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/gcoul/pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(rtos C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(rtos rtos.cpp taskManager.cpp )


pico_set_program_name(rtos "rtos")
pico_set_program_version(rtos "0.1")

pico_enable_stdio_uart(rtos 1)
pico_enable_stdio_usb(rtos 0)

# Add the standard library to the build
target_link_libraries(rtos pico_stdlib hardware_adc)

 # enable usb output, disable uart output
 pico_enable_stdio_usb(rtos 1)
 pico_enable_stdio_uart(rtos 0)

# Add the standard include files to the build
target_include_directories(rtos PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(rtos)
