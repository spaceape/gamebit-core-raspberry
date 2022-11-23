#gfx::assets

set(NAME assets)
set(ASSETS_DATA_DIR ${GLOBAL_ASSETS_DIR})

install(
  FILES
    gfx-font-8x8-00.bin
    gfx-font-8x16-00.bin
    gfx-font-8x16-01.bin
  DESTINATION
    ${ASSETS_DATA_DIR}
)
