#gamebit-core-raspberry::test

add_executable(test-blink test-blink.cpp)
target_link_libraries(test-blink pico_stdlib)

add_executable(test-graphics-basic test-graphics-basic.cpp)
target_link_libraries(test-graphics-basic pico_stdlib)

add_executable(test-graphics-0 test-graphics-0.cpp)
target_link_libraries(test-graphics-0 pico_stdlib)

add_executable(test-graphics-1 test-graphics-1.cpp)
target_link_libraries(test-graphics-1 pico_stdlib pico_multicore)

add_executable(test-graphics-2 test-graphics-2.cpp)
target_link_libraries(test-graphics-2 pico_stdlib pico_multicore)

add_executable(test-graphics-3 test-graphics-3.cpp)
target_link_libraries(test-graphics-3 pico_stdlib)
