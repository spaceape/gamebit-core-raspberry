#test::elf::arm

set(srcs
  main.c sumxy.c
)

add_library(arm-none-eabi STATIC ${srcs})
set_target_properties(arm-none-eabi PROPERTIES PREFIX "")
