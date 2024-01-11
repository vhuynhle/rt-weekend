cmake_minimum_required(VERSION 3.25)

# Ray-tracing in one weekend
project(rt-weekend)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(rtweekend INTERFACE)
target_include_directories(rtweekend INTERFACE include)

add_subdirectory(01_ppm)
add_subdirectory(02_camera)
add_subdirectory(03_sphere)
