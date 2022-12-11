# gfx
# graphics library
project(gfx)
set(NAME gfx)
set(PREFIX)

add_definitions(
  -fPIC
  -DCPU=${CPU}
  -DDPU=${DPU} -DDPU_H=<dpu/${DPU}.h>
  -DFPU=${FPU} -DFPU_H=<fpu/${FPU}.h>
  -DGPU=${GPU} -DGPU_H=<gpu/${GPU}.h>
  -DPIC
  ${HOST_DEFS}
)

include_directories(
  ${HOST_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GFX_SDK_DIR ${GLOBAL_SDK_DIR}/${NAME})

set(inc
  metrics.h ac.h dc.h driver.h
  tile.h raster.h vector.h
  surface.h
  device.h
)

set(srcs

  metrics.cpp ac.cpp dc.cpp driver.cpp tile.cpp raster.cpp vector.cpp
  
  raster/pbo.cpp
  tile/cmo.cpp tile/cso.cpp tile/cbo.cpp
  
  driver/hmm.cpp
  support/sdi.cpp
  
  surface.cpp device.cpp
  
  gfx.cpp
)

set(libs pico_stdlib host)

add_subdirectory(assets)
add_subdirectory(driver)
# add_subdirectory(image)
add_subdirectory(raster)
add_subdirectory(tile)
add_subdirectory(vector)
add_subdirectory(support)

add_library(${NAME} STATIC ${srcs})
set_target_properties(${NAME} PROPERTIES PREFIX "${PREFIX}")
target_link_libraries(${NAME} ${libs})

if(SDK)
  file(MAKE_DIRECTORY ${GFX_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${GFX_SDK_DIR}
  )
endif(SDK)

