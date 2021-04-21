#gamebit
set(GAMEBIT_SRC_DIR)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(config.in.h ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(version.in.h ${CMAKE_CURRENT_BINARY_DIR}/version.h)

set(inc)

set(srcs
  post.cpp gamebit.cpp
)

add_subdirectory(uld)

# static libraries exported for testing
add_library(gbx ${srcs})
target_link_libraries(gbx core uld)

# main gamebit image
add_executable(gamebit main.cpp ${srcs})
target_link_libraries(gamebit core uld)
