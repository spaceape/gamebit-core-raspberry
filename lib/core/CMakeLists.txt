# core
# core library; contains the support code for:
# - memory management (TODO: cleanup - direct port from my AVR codebase, RPico has much of the C++ stdlib support available)
# - serial communication
# - persistent storage
#   - SDCard driver (TODO: implement DMA/PIO, multiblock transfer)
#   - filesystem access (TODO: implement FAT16 and FS writing for FAT16/32)
# - graphics
#   - ILI9341 display driver
#   - tile && raster graphics library (WIP)
#

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DGPU_DRIVER=${GPU_DRIVER} -DGPU_FONT=${GPU_FONT} -DGPU_PALETTE=${GPU_PALETTE})

set(inc)

set(srcs

  dev/bd/bd.cpp dev/bd/spio.cpp dev/bd/drive.cpp dev/bd/fat/part.cpp
  
  dev/lcd/lcd.cpp
  dev/lcd/ili9341.cpp
  dev/lcd/gfx/dc.cpp dev/lcd/gfx/metrics.cpp
  dev/lcd/gfx/tile/cbo.cpp
  dev/lcd/gfx/raster/pbo.cpp
  dev/lcd/gfx/surface.cpp dev/lcd/gfx/canvas.cpp dev/lcd/gfx/sprite.cpp dev/lcd/gfx/scene.cpp

  sys/ios/ios.cpp sys/ios/rio.cpp sys/ios/sio.cpp sys/ios/bio.cpp sys/ios/pio.cpp sys/ios/fio.cpp
  sys/sys.cpp

  memory/memory.cpp

  error.cpp log.cpp
)

set(libs
)

add_subdirectory(dpu)
add_subdirectory(fpu)
add_subdirectory(gpu)
add_subdirectory(memory)
add_subdirectory(dev)
add_subdirectory(sys)

add_library(core ${srcs})
target_link_libraries(core pico_stdlib pico_multicore hardware_spi)
