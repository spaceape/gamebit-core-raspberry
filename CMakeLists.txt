cmake_minimum_required(VERSION 3.8.2)
set(PROJECT "gamebit-core-raspberry")
set(AUTHOR "")
set(YEAR "2020")
set(CPU "rp2040")
set(ARCH "ARM")
set(CMAKE_SYSTEM_NAME none)
set(CMAKE_SYSTEM_PROCESSOR ${CPU})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_GENERATOR "Unix Makefiles")
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(OS ${CMAKE_SYSTEM_NAME})
set(VMAJOR "0")
set(VMINOR "1")

set(DPU none)

set(FPU none)

# include only a few distinct graphics resources, to minimise memory occupancy
# values for the driver, font and palette resources are identified in the gpu.h header
# drivers are implemented in the /dev/lcd directory
# fonts and palettes defined in the dev/lcd/gfx directories
set(GPU gfx)
set(GPU_DRIVER 2)
set(GPU_FONT 8)     
set(GPU_PALETTE 8)

set(EXCEPTIONS OFF)
set(DEV ON)
set(DEBUG ON)
set(SDK OFF)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# include_directories(
#   /usr/arm-none-eabi/include
# )

include(pico_sdk_import.cmake)

project(${PROJECT})

pico_sdk_init()

add_definitions(-Wall -Wextra)

if(NOT ${CMAKE_BUILD_TYPE} EQUAL "Release")
  add_definitions(-Wno-unused-parameter)
endif()

add_definitions(-DCPU=${CPU} -DDPU=${DPU} -DGPU=${GPU} -DARCH=${ARCH})

include_directories(
  ${PICO_SDK_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/core
  ${CMAKE_CURRENT_SOURCE_DIR}/gamebit
)

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(gamebit)
add_subdirectory(test)
