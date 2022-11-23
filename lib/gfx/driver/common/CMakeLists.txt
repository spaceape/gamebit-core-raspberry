#gfx::driver::common

set(NAME common)
set(COMMON_SRC_DIR ${DRIVER_SRC_DIR}/common)

set(inc
)

if(SDK)
  file(MAKE_DIRECTORY ${COMMON_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${COMMON_SRC_DIR}
  )
endif(SDK)

