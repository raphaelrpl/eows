#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#
#  Description: Find Proj.4 include directory and library.
#
#  PROJ4_INCLUDE_DIR - where to find proj_api.h.
#  PROJ4_LIBRARY     - where to find proj libraries.
#  Proj4_FOUND       - True if Crow is found.
#

find_path(PROJ4_INCLUDE_DIR proj_api.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include)

if(UNIX)

  find_library(PROJ4_LIBRARY
               NAMES proj
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)
elseif(WIN32)

  find_library(PROJ4_LIBRARY_RELEASE
               NAMES proj_i proj
               PATH_SUFFIXES lib)

  find_library(PROJ4_LIBRARY_DEBUG
               NAMES proj_i_d proj_d
               PATH_SUFFIXES lib)

  if(PROJ4_LIBRARY_RELEASE AND PROJ4_LIBRARY_DEBUG)
    set(PROJ4_LIBRARY optimized ${PROJ4_LIBRARY_RELEASE} debug ${PROJ4_LIBRARY_DEBUG})
  elseif(PROJ4_LIBRARY_RELEASE)
    set(PROJ4_LIBRARY optimized ${PROJ4_LIBRARY_RELEASE} debug ${PROJ4_LIBRARY_RELEASE})
  elseif(PROJ4_LIBRARY_DEBUG)
    set(PROJ4_LIBRARY optimized ${PROJ4_LIBRARY_DEBUG} debug ${PROJ4_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Proj4 DEFAULT_MSG PROJ4_LIBRARY PROJ4_INCLUDE_DIR)

mark_as_advanced(PROJ4_INCLUDE_DIR PROJ4_LIBRARY)
