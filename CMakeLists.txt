cmake_minimum_required(VERSION 3.21)

# set(PICO_SDK_PATH "/opt/raspberry/pico-sdk")

include(pico_sdk_import.cmake)

project(blackops C CXX ASM)
set(PROJECT "blackops")
set(AUTHOR "")
set(YEAR "2022")
set(CPU "cortex-m0plus")
set(ARCH "ARM")
set(CMAKE_SYSTEM_NAME none)
set(CMAKE_SYSTEM_PROCESSOR ${CPU})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_GENERATOR "Unix Makefiles")
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# if(${CMAKE_BUILD_TYPE} EQUAL "Debug")
  set(CMAKE_VERBOSE_MAKEFILE ON)
  set(PICO_DEOPTIMIZED_DEBUG 1)
# endif()
# set(PICO_NO_GC_SECTIONS TRUE)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

pico_sdk_init()

set(OS ${CMAKE_SYSTEM_NAME})
set(VMAJOR "0")
set(VMINOR "1")

set(DPU none)
set(FPU none)
set(GPU gfx)

set(EXCEPTIONS OFF)
set(DEV ON)
set(DEBUG ON)
set(SDK OFF)

add_compile_options(
  -Wall
  -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
  -Wno-unused-function # we have some for the docs that aren't called
  -fno-common
  -fsigned-char
  # -falign-functions=16
  # -falign-labels=4
  # -falign-jumps=4
)

set(GLOBAL_DEFS)
set(GLOBAL_INCLUDES)
set(GLOBAL_LIBS)
set(GLOBAL_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(GLOBAL_SDK_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
set(GLOBAL_ASSETS_DIR ${CMAKE_CURRENT_BINARY_DIR}/assets)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/host
)

add_subdirectory(include)
add_subdirectory(assets)
add_subdirectory(lib)
add_subdirectory(blackops)
add_subdirectory(usr)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/blackops
)

add_subdirectory(test)
