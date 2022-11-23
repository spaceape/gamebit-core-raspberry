# host
# Host support library
# contains host support code for:
# - memory management (TODO: cleanup - direct port from my AVR codebase, RPico has much of the C++ stdlib support available)
# - serial communication
set(NAME "host")
set(PREFIX)

add_definitions(
  -DCPU=${CPU}
  -DDPU=${DPU} -DDPU_H=<dpu/${DPU}.h>
  -DFPU=${FPU} -DFPU_H=<fpu/${FPU}.h>
  -DGPU=${GPU} -DGPU_H=<gpu/${GPU}.h>
)

include_directories(
  ${PICO_SDK_PATH}/src/common/pico_base/include
  ${PICO_SDK_PATH}/src/common/pico_stdlib/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(-DGPU_DRIVER=${GPU_DRIVER} -DGPU_FONT=${GPU_FONT} -DGPU_PALETTE=${GPU_PALETTE})

set(inc)

set(srcs

  sys/adt/adt.cpp sys/adt/node.cpp sys/adt/device.cpp sys/adt/directory.cpp
  sys/ios/ios.cpp sys/ios/rio.cpp sys/ios/sio.cpp sys/ios/bio.cpp sys/ios/pio.cpp
  sys/sys.cpp

  mmi/mmi.cpp

  error.cpp log.cpp
)

set(libs
)

add_subdirectory(dpu)
add_subdirectory(fpu)
add_subdirectory(gpu)
add_subdirectory(mmi)
add_subdirectory(sys)

add_library(${NAME} ${srcs})
target_link_libraries(${NAME} pico_stdlib pico_multicore hardware_pwm hardware_spi)
