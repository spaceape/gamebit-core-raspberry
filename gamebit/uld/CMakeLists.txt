# gamebit::uld
# gamebit microloader: utilities to load different binary formats and prepare them for execution
# # Input formats:
# - ELF32
# - UF2   (TODO)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(srcs
  uld.cpp target.cpp object.cpp archive.cpp elf32.cpp
)

set(libs
  core
)

add_library(uld STATIC ${srcs})
target_link_libraries(uld ${libs})
