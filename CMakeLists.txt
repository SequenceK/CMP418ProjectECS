cmake_minimum_required(VERSION 3.0)
project(multi)
find_path(SDL2_INCLUDE_DIR SDL2/SDL.h)
find_package(SDL2 REQUIRED)
find_package(OpenMP REQUIRED)

if(OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if(MINGW)
  set(MINGW32_LIBRARY mingw32 "-mwindows" CACHE STRING "link flags for MinGW")
endif()

add_executable(multi main.cpp)

target_include_directories(multi PUBLIC ${SDL2_INCLUDE_DIR} ${OpenMP_INCLUDE_DIR})
target_link_libraries(multi ${SDL2_LIBRARIES})
