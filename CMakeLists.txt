cmake_minimum_required(VERSION  3.9 FATAL_ERROR)
project(discovery)

#=============================================================================
# Augment CMake with our custom scripts
#=============================================================================
set(EXTRA_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)


#=============================================================================
# General configuration
#=============================================================================
include(${EXTRA_MODULES_DIR}/EnsureBuildType.cmake)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 11)

include(${EXTRA_MODULES_DIR}/GetGoogleTest.cmake)

#=============================================================================
# Include subdirectories
#=============================================================================
add_subdirectory(src)


#=============================================================================
# cmake summary
#=============================================================================
message(STATUS "**************************************************************")
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "**************************************************************")
