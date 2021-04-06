# core

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(inc)

set(srcs
  dev/block.cpp dev/spio.cpp
  sys/fs/raw/filesystem.cpp sys/fs/fat/partition.cpp sys/fs/fat/node.cpp
  sys/sys.cpp sys/ios/ios.cpp sys/ios/rio.cpp sys/ios/sio.cpp sys/ios/bio.cpp sys/ios/pio.cpp
  memory/memory.cpp
  error.cpp log.cpp
)

add_subdirectory(dpu)
add_subdirectory(fpu)
add_subdirectory(gpu)
add_subdirectory(memory)
add_subdirectory(dev)
add_subdirectory(sys)

add_library(core ${srcs})
