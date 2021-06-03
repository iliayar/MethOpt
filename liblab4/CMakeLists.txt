file(GLOB SRCS src/*.cpp)

add_library(liblab4 STATIC
    ${SRCS}
)

target_include_directories(liblab4 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(include)
target_link_libraries(liblab4
  libmetopt)
