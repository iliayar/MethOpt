file(GLOB Lab3cli_SRCS src/*.cpp)
file(GLOB Lab3cli_INCL include/*.hpp)

include(FetchContent)
FetchContent_Declare(
  fire
  GIT_REPOSITORY https://github.com/kongaskristjan/fire-hpp
)
FetchContent_MakeAvailable(fire)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILE_OPTS -Wall -Wextra -pedantic -pedantic-errors -std=c++17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(../liblab3/include)
include_directories(../lib/include)

add_executable(lab3cli
  ${Lab3cli_SRCS}
  ${Lab3cli_INCL})

target_link_libraries(lab3cli
  liblab3
  fire-hpp::fire-hpp
)

add_executable(matrix_test ../liblab3/src/matrix.cpp)
