# core
# core library; contains the support code for:
# - memory management (TODO: cleanup - direct port from my AVR codebase, RPico has much of the C++ stdlib support available)
# - serial coms
# - persistent storage
#   - SDCard (TODO: implement DMA/PIO, multiblock transfer)
#   - filesystem access (TODO: implement FAT16 and FS writing for FAT16/32)
# - graphics
#   - ILI9341 display driver (TODO: have it as a proof of concept in the test directory, make it into an usable API)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(inc)

set(srcs

  dev/bd.cpp  dev/spio/spio.cpp

  sys/raw/drive.cpp
  sys/fat/part.cpp
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
target_link_libraries(core pico_stdlib hardware_spi)
