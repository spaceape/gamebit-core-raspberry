
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLOBAL_LIBRARY_DIR}/uld
  ${GLOBAL_LIBRARY_DIR}/fat
  ${GLOBAL_LIBRARY_DIR}/fat/ff14a
  ${GLOBAL_LIBRARY_DIR}/fat/sd_driver
)

add_subdirectory(share)

add_executable(test-blink test-blink.cpp)
target_link_libraries(test-blink pico_stdlib)

add_executable(test-elf-1 test-elf-1.cpp hw_config.c)
target_link_libraries(test-elf-1 fat uld hardware_spi hardware_dma hardware_rtc pico_stdlib)
target_compile_definitions(
  test-elf-1
  PRIVATE
      PICO_STACK_SIZE=4096
      PICO_HEAP_SIZE=196608
)

add_executable(test-elf-2 test-elf-2.cpp hw_config.c)
target_link_libraries(test-elf-2 fat uld hardware_spi hardware_dma hardware_rtc pico_stdlib)
target_compile_definitions(
  test-elf-2
  PRIVATE
      PICO_STACK_SIZE=4096
      PICO_HEAP_SIZE=196608
)

add_executable(test-elf-3
  share/sys.cpp
  share/file.cpp
  share/string.cpp
  test-elf-3.cpp
  hw_config.c
)
target_link_libraries(test-elf-3 fat uld blackops hardware_spi hardware_dma hardware_rtc pico_stdlib)
target_compile_definitions(
  test-elf-3
  PRIVATE
      PICO_STACK_SIZE=4096
      PICO_HEAP_SIZE=196608
)

add_executable(test-graphics-1
  share/sys.cpp
  share/file.cpp
  share/gfx.cpp
  test-graphics-1.cpp hw_config.c
)
target_link_libraries(test-graphics-1 pico_stdlib hardware_spi hardware_dma hardware_rtc blackops fat gfx)
# target_compile_definitions(
#   test-graphics-1
#   PRIVATE
#       PICO_STACK_SIZE=4096
#       PICO_HEAP_SIZE=131072
# )
# pico_set_binary_type(test-graphics-1 no_flash)
