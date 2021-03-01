file(GLOB SRCS src/*.cpp)

add_library(libmetopt STATIC
    ${SRCS}
)

target_include_directories(libmetopt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
