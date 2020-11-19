# using https://cliutils.gitlab.io/modern-cmake/chapters/basics.html to get started with cmake

# this file combines the differen parts of the engine

# -----------------------------------------------
# making sure cmake works as intended
#
# -----------------------------------------------
cmake_minimum_required(VERSION 3.7...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# ------------------------------------------------
# adding the different parts of the engine 
#
# ------------------------------------------------
add_subdirectory(graphics)

# ------------------------------------------------
# adding the example projects
#
# ------------------------------------------------
add_subdirectory(examples)