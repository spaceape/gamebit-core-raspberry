#gamebit
set(GAMEBIT_SRC_DIR)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(config.in.h ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(version.in.h ${CMAKE_CURRENT_BINARY_DIR}/version.h)

set(inc)

set(srcs
  com.cpp io.cpp
  post.cpp gamebit.cpp
)

add_subdirectory(uld)

# static libraries exported for testing
add_library(gbcom com.cpp)
target_link_libraries(gbcom pico_stdlib)
add_library(gbio io.cpp)
target_link_libraries(gbio core)

# main gamebit image
add_executable(gamebit ${srcs})
target_link_libraries(gamebit core pico_stdlib)
