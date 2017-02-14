#
#  Author: Gilberto Ribeiro de Queiroz
#
#  Description: Find SciDB include directory and C++ client library.
#
#  SCIDB_INCLUDE_DIR    -> where to find SciDBAPI.h and other header files.
#  SCIDB_CLIENT_LIBRARY -> C++ client library to link to.
#  SCIDB_FOUND          -> True if SciDB C++ client library is found.
#

find_path(SCIDB_INCLUDE_DIR SciDBAPI.h
          PATHS /usr
                /usr/local
                /opt/scidb/15.12
                /opt/scidb/15.7
                /opt/scidb/14.12
                /opt/scidb/14.3
                /opt/scidb/13.12
          PATH_SUFFIXES include
                        scidb/include)

if(NOT APPLE)
  find_library(SCIDB_CLIENT_LIBRARY
               NAMES scidbclient
               PATHS /usr
                     /usr/local
                     /opt/scidb/15.12
                     /opt/scidb/15.7
                     /opt/scidb/14.12
                     /opt/scidb/14.3
                     /opt/scidb/13.12
               PATH_SUFFIXES lib
                             scidb/lib)

endif()

include(FindPackageHandleStandardArgs)

if(APPLE)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(SciDB DEFAULT_MSG SCIDB_INCLUDE_DIR)
  mark_as_advanced(SCIDB_INCLUDE_DIR)
else()
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(SciDB DEFAULT_MSG SCIDB_CLIENT_LIBRARY SCIDB_INCLUDE_DIR)
  mark_as_advanced(SCIDB_INCLUDE_DIR SCIDB_CLIENT_LIBRARY)
endif()



