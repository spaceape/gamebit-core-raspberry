# FatFS_SPI
set(NAME "fat")
set(PREFIX)

add_definitions()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/ff14a
  ${CMAKE_CURRENT_SOURCE_DIR}/sd_driver
)

set(inc
)

set(srcs
  
  sd_driver/sd_spi.c sd_driver/demo_logging.c sd_driver/spi.c 
  sd_driver/sd_card.c
  sd_driver/crc.c 

  ff14a/ffsystem.c ff14a/ffunicode.c ff14a/ff.c

  glue.c
  
  f_util.c
  ff_stdio.c
  my_debug.c
  rtc.c
  # hw_config.c
)

add_library(${NAME} ${srcs})
target_link_libraries(${NAME} pico_stdlib hardware_spi hardware_dma hardware_rtc)
