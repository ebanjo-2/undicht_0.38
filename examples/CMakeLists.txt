# using https://cliutils.gitlab.io/modern-cmake/chapters/basics.html to get started with cmake
cmake_minimum_required(VERSION 3.7...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(examples VERSION 0.38 DESCRIPTION "examples for testing the engine" LANGUAGES CXX)
add_executable(first_test src/main.cpp)