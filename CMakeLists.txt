cmake_minimum_required(VERSION 3.10)
project(cpp_utils)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(iterator_traits src/iterator_traits.cpp)
add_executable(partial_specialization src/partial_specialization.cpp)
add_executable(observer_pattern src/observer_pattern.cpp)
add_executable(command_pattern src/command_pattern.cpp)
