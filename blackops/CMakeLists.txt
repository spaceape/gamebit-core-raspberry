#blackops source dir
set(NAME "blackops")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
set(USE_ILI9341S TRUE)
set(PREFIX)

add_definitions(
  -DCPU=${CPU}
  -DDPU=${DPU} -DDPU_H=<dpu/${DPU}.h>
  -DFPU=${FPU} -DFPU_H=<fpu/${FPU}.h>
  -DGPU=${GPU} -DGPU_H=<gpu/${GPU}.h>
)

include_directories(
  ${GLOBAL_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(config.in.h ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(version.in.h ${CMAKE_CURRENT_BINARY_DIR}/version.h)

add_subdirectory(dev)

set(inc)

set(srcs

    dev/bus.cpp dev/bd.cpp dev/bd/raw/crc.cpp
    dev/spi.cpp
    dev/uart.cpp
    dev/sds.cpp
    dev/file.cpp

    post.cpp calibration.cpp configuration.cpp
    controller.cpp
    blackops.cpp
)

set(libs
  pico_stdlib host
)

# link with the ILI9341 SPI graphics driver
if(USE_ILI9341S)
  set(srcs ${srcs} dev/ili9341s.cpp)
  set(libs ${libs} gfx)
endif(USE_ILI9341S)

# main blackops image
add_library(blackops STATIC ${srcs})
target_link_libraries(blackops ${libs})

# main blackops image
add_executable(blackops-core-raspberry ${srcs} main.cpp)
target_compile_definitions(
  blackops-core-raspberry
  PRIVATE
    PICO_STACK_SIZE=4096
    PICO_HEAP_SIZE=131072
)
target_link_libraries(blackops-core-raspberry blackops ${libs} emc)
# pico_add_extra_outputs(blackops-core-raspberry)

pico_enable_stdio_usb(blackops-core-raspberry ON)
