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

add_executable(lab4cli
  src/main.cpp)

target_link_libraries(lab4cli
  liblab3
  liblab4
  libmetopt
  fire-hpp::fire-hpp
)
