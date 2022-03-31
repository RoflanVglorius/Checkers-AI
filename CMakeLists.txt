cmake_minimum_required(VERSION 3.17)
project(checkers)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(lib)

FILE(GLOB include "${checkers_SOURCE_DIR}/include/*.h")

add_library(
        dep_lib SHARED main.cpp ${include}
)

add_executable(checkers main.cpp)