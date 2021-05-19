file(GLOB SRCS src/*.cpp)

add_library(liblab3 STATIC
    ${SRCS}
)

target_include_directories(liblab3 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(include)
target_link_libraries(liblab3
  libmetopt)
