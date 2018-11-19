cmake_minimum_required(VERSION 3.0)
project(multi)
find_path(SDL2_INCLUDE_DIR SDL2/SDL.h)
find_package(SDL2 REQUIRED)
add_executable(multi main.cpp)
target_include_directories(multi PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(multi SDL2::SDL2 SDL2::SDL2main)