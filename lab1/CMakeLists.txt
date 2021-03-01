file(GLOB Lab1_SRCS src/*.cpp)
file(GLOB Lab1_INCL include/*.hpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILE_OPTS -Wall -Wextra -pedantic -pedantic-errors)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(MetOptLab1
  ${Lab1_SRCS}
  ${Lab1_INCL})

target_link_libraries(MetOptLab1
  libmetopt
  Qt5::Widgets
)
