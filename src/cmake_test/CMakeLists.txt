#
# Test EOWS CMake distributable support.
#
cmake_minimum_required(VERSION 3.1.2)

project(myproject)

find_package(eows 0.1.0)

# full provided version string
message(STATUS "EOWS CMake Test: eows_VERSION = ${eows_VERSION}")

# major version if provided, else 0
message(STATUS "EOWS CMake Test: eows_VERSION_MAJOR = ${eows_VERSION_MAJOR}")

# minor version if provided, else 0
message(STATUS "EOWS CMake Test: eows_VERSION_MINOR = ${eows_VERSION_MINOR}")

# patch version if provided, else 0
message(STATUS "EOWS CMake Test: eows_VERSION_PATCH = ${eows_VERSION_PATCH}")

# tweak version if provided, else 0
message(STATUS "EOWS CMake Test: eows_VERSION_TWEAK = ${eows_VERSION_TWEAK}")

# number of version components, 0 to 4
message(STATUS "EOWS CMake Test: eows_VERSION_COUNT = ${eows_VERSION_COUNT}")

# EOWS_INCLUDE_DIRS
message(STATUS "EOWS CMake Test: EOWS_INCLUDE_DIRS = ${EOWS_INCLUDE_DIRS}")

# eows_core
if(TARGET eows_core)
    message(STATUS "EOWS CMake Test: eows_core is a target")
else()
    message(STATUS "EOWS CMake Test: eows_core is not a target")
endif()
