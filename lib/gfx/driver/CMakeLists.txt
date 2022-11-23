#gfx::driver

set(NAME driver)
set(DRIVER_SDK_DIR ${GFX_SDK_DIR}/driver)

set(inc
  hmm.h
)

add_subdirectory(common)

if(SDK)
  file(MAKE_DIRECTORY ${DRIVER_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${DRIVER_SDK_DIR}
  )
endif(SDK)

