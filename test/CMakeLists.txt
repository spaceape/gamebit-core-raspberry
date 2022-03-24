#gamebit-core-raspberry::test

add_executable(test-blink test-blink.cpp)
target_link_libraries(test-blink pico_stdlib)

add_executable(test-lcd-1 test-lcd-1.cpp)
target_link_libraries(test-lcd-1 core gbx)

# add_executable(test-graphics-basic test-graphics-basic.cpp)
# target_link_libraries(test-graphics-basic pico_stdlib)

# add_executable(test-graphics-0 test-graphics-0.cpp)
# target_link_libraries(test-graphics-0 pico_stdlib)

add_executable(test-graphics-1 test-graphics-1.cpp)
target_link_libraries(test-graphics-1 pico_stdlib pico_multicore)

# add_executable(test-graphics-2 test-graphics-2.cpp)
# target_link_libraries(test-graphics-2 pico_stdlib pico_multicore)

# add_executable(test-graphics-3 test-graphics-3.cpp)
# target_link_libraries(test-graphics-3 pico_stdlib pico_multicore)

add_executable(test-graphics-4 test-graphics-4.cpp LCD_ILI9341.cpp)
target_link_libraries(test-graphics-4 pico_stdlib)

# add_executable(test-spi-0 test-spi-0.cpp)
# target_link_libraries(test-spi-0 pico_stdlib hardware_spi)

add_executable(test-sound-1 test-sound-1.cpp)
target_link_libraries(test-sound-1 core gbx)

add_executable(test-fat-1 test-fat-1.cpp)
target_link_libraries(test-fat-1 core gbx pico_stdio)

add_executable(test-elf-1 test-elf-1.cpp smp.cpp)
target_link_libraries(test-elf-1 core gbx)

add_subdirectory(elf)
