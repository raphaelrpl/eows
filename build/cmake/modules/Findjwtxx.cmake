#
#  Author: Raphael Willian da Costa
#
#  Description: Find jwtxx include directory and library.
#
#  JWTXX_INCLUDE_DIR - where to find jwt.h.
#  JWTXX_LIBRARY     - where to find jwtxx library.
#  jwtxx_FOUND       - True if jwtxx is found.
#

find_path(JWTXX_INCLUDE_DIR jwt.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        include/jwtxx)

find_library(JWTXX_LIBRARY
             NAMES jwtxx
             PATHS /usr
                   /usr/local
             PATH_SUFFIXES lib)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(jwtxx DEFAULT_MSG JWTXX_LIBRARY JWTXX_INCLUDE_DIR)

mark_as_advanced(JWTXX_INCLUDE_DIR JWTXX_LIBRARY)