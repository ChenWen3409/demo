cmake_minimum_required(VERSION 3.5)

# PROJECT SETUP
project(demo)
set(CMAKE_VERBOSE_MAKEFILE ON)

# PROJECT OPTIONS FOR APP
option(BUILD_RDKDemo "Build RDKDemo app" OFF)

# C++14 required
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

# set default build type
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/tools/set_default_build_type.cmake)

# Add all app-level CMake projects as subdirectories,
if(${BUILD_RDKDemo})
    add_subdirectory(${CMAKE_SOURCE_DIR}/app/RDKDemo)
endif()