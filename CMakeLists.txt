cmake_minimum_required(VERSION 3.28)
project(GameOfLife LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)
add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_26)
target_link_libraries(main PRIVATE SFML::Graphics)
