
set(VECTOR_SDK_DIR ${GFX_SDK_DIR}/vector)

set(inc
)

if(SDK)
  file(MAKE_DIRECTORY ${VECTOR_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${VECTOR_SDK_DIR}
  )
endif(SDK)
