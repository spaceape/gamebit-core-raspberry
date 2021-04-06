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
set(CMAKE_CXX_COMPILER_WORKS 1)

include_directories(
  /usr/arm-none-eabi/include
)

include(pico_sdk_import.cmake)

project(${PROJECT})

pico_sdk_init()

add_definitions(-Wall -Wextra)

include_directories(
  ${PICO_SDK_PATH}
  ${PICO_SDK_PATH}/src/common/pico_base/include
  ${PICO_SDK_PATH}/src/common/pico_time/include

  ${PICO_SDK_PATH}/src/rp2040/hardware_base/include
  ${PICO_SDK_PATH}/src/rp2040/hardware_regs/include
  ${PICO_SDK_PATH}/src/rp2040/hardware_structs/include

  ${PICO_SDK_PATH}/src/rp2_common/hardware_base/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_spi/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_timer/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_platform/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_multicore/include

  ${PICO_SDK_PATH}/host/pico_platform/include
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/core
  ${CMAKE_CURRENT_BINARY_DIR}/generated/pico_base
)

# add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(gamebit)
add_subdirectory(test)
