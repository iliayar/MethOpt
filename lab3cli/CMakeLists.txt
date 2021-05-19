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

add_executable(lab3cli
  src/main.cpp
  ${Lab3cli_INCL})

add_executable(genTests
  src/genTests.cpp
  ${Lab3cli_INCL})

target_link_libraries(lab3cli
  liblab3
  libmetopt
  fire-hpp::fire-hpp
)

target_link_libraries(genTests
  liblab3
  fire-hpp::fire-hpp
)
