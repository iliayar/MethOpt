file(GLOB Lab2cli_SRCS src/*.cpp)
file(GLOB Lab2cli_INCL include/*.hpp)

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

add_executable(lab2cli
  ${Lab2cli_SRCS}
  ${Lab2cli_INCL})

target_link_libraries(lab2cli
  libmetopt
  fire-hpp::fire-hpp
)
