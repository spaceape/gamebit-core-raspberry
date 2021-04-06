set(NAME fs)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(FS_SRC_DIR ${SYS_SRC_DIR}/${NAME})

set(inc
)

add_subdirectory(raw)
add_subdirectory(fat)

if(SDK)
  file(MAKE_DIRECTORY ${SYS_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${FS_SRC_DIR}
  )
endif(SDK)
