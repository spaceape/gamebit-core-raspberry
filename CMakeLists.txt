cmake_minimum_required(VERSION 3.8.2)
set(PROJECT "gamebit-core-raspberry")
set(AUTHOR "")
set(YEAR "2020")
set(CPU rp2040)
set(CMAKE_SYSTEM_NAME none)
set(CMAKE_SYSTEM_PROCESSOR ${CPU})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_GENERATOR "Unix Makefiles")
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(OS ${CMAKE_SYSTEM_NAME})
set(VMAJOR "0")
set(VMINOR "1")
set(DPU none)
set(FPU none)
set(GPU none)
# set(EXCEPTIONS 1)
set(LINUX 0)        # autodetected, always set to "0" here
set(WINDOWS 0)      # autodetected, always set to "0" here
set(INSTRUMENT 0)
set(DEV 1)
set(DEBUG 1)
set(SDK 0)
set(CMAKE_C_COMPILER_WORKS 1)

include_directories(
  /usr/arm-none-eabi/include
)

include(pico_sdk_import.cmake)

project(${PROJECT})

pico_sdk_init()

add_definitions(-Wall -Wextra)

include_directories(
  ${PICO_SDK_PATH}/src/rp2_common/pico_multicore/include
)

add_subdirectory(lib)
add_subdirectory(gamebit)
add_subdirectory(test)
