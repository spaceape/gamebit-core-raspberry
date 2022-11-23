set(NAME ios)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(IOS_SRC_DIR ${SYS_SRC_DIR}/${NAME})

set(inc
  rio.h sio.h bio.h pio.h
)

if(SDK)
  file(MAKE_DIRECTORY ${SYS_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${IOS_SRC_DIR}
  )
endif(SDK)
