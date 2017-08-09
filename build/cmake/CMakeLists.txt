#
#  Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of Earth Observation Web Services (EOWS).
#
#  EOWS is free software: you can
#  redistribute it and/or modify it under the terms of the
#  GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  EOWS is distributed in the hope that
#  it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with EOWS. See LICENSE. If not, write to
#  e-sensing team at <esensing-team@dpi.inpe.br>.
#
#  Description: Main CMakeLists.txt of EOWS build system.
#
#  Author: Gilberto Ribeiro de Queiroz
#

#
# Warning: EOWS requires at least CMake 2.8
#
cmake_minimum_required(VERSION 2.8)

#
# Project codename: eows
#
project(eows)

#
# Variables for the system versioning
#
set(EOWS_VERSION_MAJOR 0)
set(EOWS_VERSION_MINOR 5)
set(EOWS_VERSION_PATCH 0)
set(EOWS_VERSION_STATUS "alpha1")
set(EOWS_VERSION_NUMBER "0x000500")

if(EOWS_VERSION_STATUS)
    set(EOWS_VERSION_STRING "${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR}.${EOWS_VERSION_PATCH}-${EOWS_VERSION_STATUS}")
else()
    set(EOWS_VERSION_STRING "${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR}.${EOWS_VERSION_PATCH}-${EOWS_VERSION_STATUS}")
endif()

if(EOWS_BUILD_NUMBER)
    set(EOWS_VERSION_STRING "${EOWS_VERSION_STRING}-build-${EOWS_BUILD_NUMBER}")
endif()

message(STATUS "EOWS: build version = '${EOWS_VERSION_STRING}'")

#
# Find the EOWS absolute paths
#
get_filename_component(EOWS_ABSOLUTE_ROOT_DIR "../../" ABSOLUTE)

message(STATUS "EOWS: codebase path in: '${EOWS_ABSOLUTE_ROOT_DIR}'")

#
# Common definitions due to the Boost Libraries
#
add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_UBLAS_TYPE_CHECK=0 -DBOOST_LOG_DYN_LINK )

#
# Set maximum instantiation depth for template classes to 32
#
if(APPLE)
    add_definitions(-ftemplate-depth-256)
endif()

#
# Compiler options: warning and C++ 11
#
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER MATCHES ".*clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
endif()

#
# Add source tree directory to search for CMake modules
#
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules")

# Build tree directory will be needed in all projects.
include_directories(${PROJECT_BINARY_DIR})

message(STATUS "EOWS: adding general include directory: '${PROJECT_BINARY_DIR}'")

#
# Warning: SciDB API requires the definition of PROJECT_ROOT variable
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPROJECT_ROOT='\"${EOWS_ABSOLUTE_ROOT_DIR}/src\"'")

#
# Set output directory for libraries and executables.
#
if(CMAKE_GENERATOR STREQUAL Xcode)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    message(STATUS "EOWS: setting output directory to: '${CMAKE_BINARY_DIR}'")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    message(STATUS "EOWS: setting output directory to: '${CMAKE_BINARY_DIR}/bin'")
endif()

#
# Enable CTest
#
enable_testing()
message(STATUS "EOWS: CTest enabled!")

#
# Include CMake's support for a macro that provides an option dependent on other options.
#
include(CMakeDependentOption)

#
# Search for 3rd-party libraries.
#
find_package(Boost 1.54.0 QUIET REQUIRED system date_time chrono log timer filesystem program_options thread)

if(Boost_FOUND)
    message(STATUS "EOWS: Boost Libraries found!")
    include_directories(SYSTEM ${Boost_INCLUDE_DIR})
else()
    message(FATAL_ERROR "EOWS: could not find required Boost libraries. Please, refer to the EOWS build documentation!")
endif()

find_package(SciDB QUIET)

if(SciDB_FOUND)
    message(STATUS "EOWS: SciDB runtime found!")
    include_directories(SYSTEM ${SCIDB_INCLUDE_DIR})
    add_definitions(-DSCIDB_CLIENT)

    if(NOT DEFINED EOWS_SCIDB_MAJOR_VERSION)
      set(EOWS_SCIDB_MAJOR_VERSION "15" CACHE STRING "What is the SciDB major version?")
    endif()

else()
    message(STATUS "EOWS: SciDB runtime was not found. Building without it!")
endif()

find_package(Threads REQUIRED)

if(Threads_FOUND)
    message(STATUS "EOWS: thread library system found!")
else()
    message(FATAL_ERROR "EOWS: could not find required thread library system. Please, refer to the EOWS build documentation!")
endif()

find_package(cppnetlib QUIET)

if(cppnetlib_FOUND)
    message(STATUS "EOWS: C++ Network Library found!")
    include_directories(${CPPNETLIB_INCLUDE_DIRS})
    add_definitions(-DEOWS_CPPNETLIB_ENABLED)
else()
    message(STATUS "EOWS: could not find C++ Network Library. Building without it!")
endif()

find_package(Crow QUIET)

if(Crow_FOUND)
    message(STATUS "EOWS: Crow Library found!")
    include_directories(SYSTEM ${CROW_INCLUDE_DIR})
    add_definitions(-DEOWS_CROW_ENABLED)
else()
    if(cppnetlib_FOUND)
      message(STATUS "EOWS: could not find Crow Library. Building without it!")
    else()
      message(FATAL_ERROR "EOWS: could not find C++ Network Library nor Crow Library. At least one of them are required to build. Please, refer to the EOWS build documentation!")
    endif()
endif()

find_package(RapidJSON REQUIRED)

if(RapidJSON_FOUND)
    message(STATUS "EOWS: RapidJSON Library found!")
    include_directories(${RAPIDJSON_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "EOWS: could not find required RapidJSON Library. Please, refer to the EOWS build documentation!")
endif()

find_package(RapidXml QUIET)

if(RAPIDXML_FOUND)
    message(STATUS "EOWS: RapidXML Library found!")
    include_directories(${RAPIDXML_INCLUDE_DIR})
    add_definitions(-DEOWS_RAPIDXML_ENABLED)
else()
    message(STATUS "EOWS: could not find RapidXML Library. Building without it!")
endif()

find_package(Proj4 QUIET)

if(Proj4_FOUND)
    message(STATUS "EOWS: Proj.4 Library found!")
    include_directories(${PROJ4_INCLUDE_DIR})
else()
    message(STATUS "EOWS: could not find Proj.4 Library. Please, refer to the EOWS build documentation!")
endif()

find_package(LibGD QUIET)

if(LibGD_FOUND)
    message(STATUS "EOWS: GD Library found!")
    include_directories(${LIBGD_INCLUDE_DIR})
    add_definitions(-DEOWS_LIBGD_ENABLED)
else()
    message(STATUS "EOWS: could not find GD Library. Building without it!")
endif()

find_package(GDAL QUIET)

if(GDAL_FOUND)
    message(STATUS "EOWS: GDAL2 Library found")
    include_directories(${GDAL_INCLUDE_DIR})
else()
    message(STATUS "EOWS: Could not find GDAL Library. Building without it")
endif()

#
# Build options.
#
CMAKE_DEPENDENT_OPTION(EOWS_HTTP_CROW_ENABLED "Build http-server ON top of Crow Library?" ON "Crow_FOUND" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_HTTP_CPPNETLIB_ENABLED "Build http-server OFF top of C++ Network Library?" ON "cppnetlib_FOUND" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_PROJ4_ENABLED "Build Proj.4 Support?" ON "Proj4_FOUND" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_GDAL2_ENABLED "Build GDAL2 Support?" ON "GDAL_FOUND" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_SCIDB_ENABLED "Build SciDB runtime support for EOWS?" ON "SciDB_FOUND" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_GEOARRAY_ENABLED "Build GeoArray module?" ON "EOWS_SCIDB_ENABLED;EOWS_PROJ4_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_GEOCACHE_ENABLED "Build GeoCache module?" OFF "EOWS_GEOARRAY_ENABLED;EOWS_SCIDB_ENABLED" OFF)

option(EOWS_APP_SERVER_ENABLED "Build application web server?" ON)

option(EOWS_SERVICE_ECHO_ENABLED "Build echo service?" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_OGC_OWS_ENABLED "Build OGC OWS support?" OFF "EOWS_GEOARRAY_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_SERVICE_WTSS_ENABLED "Build WTSS service?" ON "EOWS_GEOARRAY_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_SERVICE_WCS_ENABLED "Build OGC WCS service?" OFF "RAPIDXML_FOUND;EOWS_OGC_OWS_ENABLED;EOWS_GDAL2_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_SERVICE_WMS_ENABLED "Build OGC WMS service?" OFF "LibGD_FOUND;EOWS_OGC_OWS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(EOWS_SERVICE_WTSCS_ENABLED "Build Web Time Series Classification service?" ON "EOWS_GEOARRAY_ENABLED" OFF)

#
# Define installation directories
#
set(EOWS_INSTALL_LIB_DIR lib)
set(EOWS_INSTALL_BIN_DIR bin)
set(EOWS_INSTALL_INCLUDE_DIR include)
set(EOWS_INSTALL_SHARE_DIR share)

if(WIN32 AND NOT CYGWIN)
  set(EOWS_INSTALL_CMAKE_DIR cmake)
else()
  set(EOWS_INSTALL_CMAKE_DIR lib/cmake/eows-${EOWS_VERSION_STRING})
endif()

# Detecting User home directory to create EOWS log execution
IF(UNIX)
  set(EOWS_USER_HOME "$ENV{HOME}")
ELSE()
  set(EOWS_USER_HOME "$ENV{USERPROFILE}")
ENDIF()

#
# Make relative paths absolute: needed to redistribute CMake files
#
foreach(p LIB BIN INCLUDE SHARE CMAKE)
    set(var EOWS_INSTALL_${p}_DIR)

    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()

    #message(STATUS "EOWS: var = ${${var}}")
endforeach()

#
# Configuration of general source code files that must be installed
# if devel component is installed.
#
configure_file(${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/build_config.hpp.in
               ${PROJECT_BINARY_DIR}/build_config.hpp @ONLY)

configure_file(${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/version.hpp.in
               ${PROJECT_BINARY_DIR}/eows_version.hpp @ONLY)

configure_file(${EOWS_ABSOLUTE_ROOT_DIR}/share/eows/config/eows.json.in
               ${EOWS_ABSOLUTE_ROOT_DIR}/share/eows/config/eows.json @ONLY)

install(FILES ${PROJECT_BINARY_DIR}/build_config.hpp
              ${PROJECT_BINARY_DIR}/eows_version.hpp
              ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/core.hpp
              ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/exception.hpp
              ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/version.hpp
        DESTINATION "${EOWS_INSTALL_INCLUDE_DIR}"
        COMPONENT devel)

install(DIRECTORY ${EOWS_ABSOLUTE_ROOT_DIR}/share/
        DESTINATION "${EOWS_INSTALL_SHARE_DIR}"
        COMPONENT runtime)

#
# Modules to build
#
add_subdirectory(core)

if(EOWS_HTTP_CPPNETLIB_ENABLED)
  add_definitions(-DEOWS_HTTP_CPPNETLIB_ENABLED)
  add_subdirectory(http_cppnetlib)
endif()

if(EOWS_HTTP_CROW_ENABLED)
  add_definitions(-DEOWS_HTTP_CROW_ENABLED)
  add_subdirectory(http_crow)
endif()

if(EOWS_PROJ4_ENABLED)
  add_definitions(-DEOWS_PROJ4_ENABLED)
  add_subdirectory(proj4)
endif()

if(EOWS_GDAL2_ENABLED)
  add_definitions(-DEOWS_GDAL2_ENABLED)
  add_subdirectory(gdal2)
endif()

if(EOWS_SCIDB_ENABLED)
  add_definitions(-DEOWS_SCIDB_ENABLED -DEOWS_SCIDB_MAJOR_VERSION=${EOWS_SCIDB_MAJOR_VERSION})
  add_subdirectory(scidb)
endif()

if(EOWS_GEOARRAY_ENABLED)
  add_definitions(-DEOWS_GEOARRAY_ENABLED)
  add_subdirectory(geoarray)
endif()

if(EOWS_GEOCACHE_ENABLED)
  add_definitions(-DEOWS_GEOCACHE_ENABLED)
  add_subdirectory(geocache)
endif()

if(EOWS_OGC_OWS_ENABLED)
  add_definitions(-DEOWS_OGC_OWS_ENABLED)
  add_subdirectory(ows)
endif()

if(EOWS_SERVICE_ECHO_ENABLED)
  add_definitions(-DEOWS_SERVICE_ECHO_ENABLED)
  add_subdirectory(echo)
endif()

if(EOWS_SERVICE_WTSS_ENABLED)
  add_definitions(-DEOWS_SERVICE_WTSS_ENABLED)
  add_subdirectory(wtss)
endif()

if(EOWS_SERVICE_WCS_ENABLED)
  add_definitions(-DEOWS_SERVICE_WCS_ENABLED)
  add_subdirectory(wcs)
endif()

if(EOWS_SERVICE_WMS_ENABLED)
  add_definitions(-DEOWS_SERVICE_WMS_ENABLED)
  add_subdirectory(wms)
endif()

if(EOWS_SERVICE_WTSCS_ENABLED)
  add_definitions(-DEOWS_SERVICE_WTSCS_ENABLED)
  add_subdirectory(wtscs)
endif()

if(EOWS_APP_SERVER_ENABLED)
  add_subdirectory(app_server)
endif()

#
# Export all targets information to the build-tree.
# Tip:
#   * Another way would be the following command: export(TARGETS ${EOWS_EXPORT_TARGETS} FILE "${PROJECT_BINARY_DIR}/eows-targets.cmake")
#   * In this case we would need a list such as: set(EOWS_EXPORT_TARGETS "")
#                                                list(APPEND EOWS_EXPORT_TARGETS "eows_core")
#
export(EXPORT eows-targets FILE "${PROJECT_BINARY_DIR}/eows-targets.cmake")

#
# Register EOWS' current build directory in the CMake user 
# package registry. This way the find_package command may consider
# this build directory while searching for the package eows.
#
export(PACKAGE eows)

#
# find_package(eows) command will rely on this archive in order to
# find out EOWS version.
#
configure_file(${PROJECT_SOURCE_DIR}/eows-config-version.cmake.in
               ${PROJECT_BINARY_DIR}/eows-config-version.cmake @ONLY)

#
# find_package(eows) will rely on this file when cross compiling
# in the build tree. Note that EOWS_CONF_INCLUDE_DIRS will point
# to codebase and build-tree.
#
set(EOWS_CONF_INCLUDE_DIRS "${EOWS_ABSOLUTE_ROOT_DIR}/src" "${PROJECT_BINARY_DIR}")
#message(STATUS "EOWS: EOWS_CONF_INCLUDE_DIRS = ${EOWS_CONF_INCLUDE_DIRS}")

configure_file(${PROJECT_SOURCE_DIR}/eows-config.cmake.in
               ${PROJECT_BINARY_DIR}/eows-config.cmake @ONLY)

#
# find_package(eows) will rely on this file when cross compiling
# in the install tree.
#
file(RELATIVE_PATH EOWS_REL_INCLUDE_DIR "${EOWS_INSTALL_CMAKE_DIR}" "${EOWS_INSTALL_INCLUDE_DIR}")

#message(STATUS "EOWS: EOWS_REL_INCLUDE_DIR = ${EOWS_REL_INCLUDE_DIR}")

set(EOWS_CONF_INCLUDE_DIRS "\${EOWS_CMAKE_DIR}/${EOWS_REL_INCLUDE_DIR}")
#message(STATUS "EOWS: EOWS_CONF_INCLUDE_DIRS = ${EOWS_CONF_INCLUDE_DIRS}")

configure_file(${PROJECT_SOURCE_DIR}/eows-config.cmake.in
               ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/eows-config.cmake @ONLY)

#
# Install CMake support files
#
install(FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/eows-config.cmake
              ${PROJECT_BINARY_DIR}/eows-config-version.cmake
        DESTINATION "${EOWS_INSTALL_CMAKE_DIR}"
        COMPONENT devel)

#
# Export all targets information to the install-tree
#
install(EXPORT eows-targets
        DESTINATION "${EOWS_INSTALL_CMAKE_DIR}"
        FILE eows-targets.cmake
        COMPONENT devel)

