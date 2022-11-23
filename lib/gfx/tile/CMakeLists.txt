
set(TILE_SDK_DIR ${GFX_SDK_DIR}/tile)

set(inc
  cmo.h cso.h cbo.h
)

if(SDK)
  file(MAKE_DIRECTORY ${TILE_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${TILE_SDK_DIR}
  )
endif(SDK)
