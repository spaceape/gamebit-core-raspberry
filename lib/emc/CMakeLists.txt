# emc
# Embedded Machine Control library
set(NAME "emc")
set(PREFIX)

add_definitions(
  -DCPU=${CPU}
  -DDPU=${DPU} -DDPU_H=<dpu/${DPU}.h>
  -DFPU=${FPU} -DFPU_H=<fpu/${FPU}.h>
  -DGPU=${GPU} -DGPU_H=<gpu/${GPU}.h>
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(config.in.h ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(inc
  protocol.h
)

set(srcs
  command.cpp timer.cpp
  gateway.cpp session.cpp reactor.cpp
  emc.cpp
)

set(libs
)

add_library(${NAME} ${srcs})
target_link_libraries(${NAME} pico_stdlib host)
