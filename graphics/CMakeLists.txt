# using https://cliutils.gitlab.io/modern-cmake/chapters/basics.html to get started with cmake
cmake_minimum_required(VERSION 3.7...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(undicht_graphics VERSION 0.38 DESCRIPTION "tools to create windows and graphics (2D/3D)" LANGUAGES CXX)

# adding the source files to the project
add_library(undicht_graphics STATIC src/core/window/window.cpp include/core/window/window.h)
target_include_directories(undicht_graphics PUBLIC include)
target_compile_features(undicht_graphics PUBLIC cxx_std_11)

# adding external libraries
add_subdirectory(extern/glfw)