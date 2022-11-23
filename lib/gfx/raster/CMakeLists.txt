
set(RASTER_SDK_DIR ${GFX_SDK_DIR}/raster)

set(inc
  pbo.h
)

if(SDK)
  file(MAKE_DIRECTORY ${RASTER_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${RASTER_SDK_DIR}
  )
endif(SDK)
