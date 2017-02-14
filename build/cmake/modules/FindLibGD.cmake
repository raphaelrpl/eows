#
#  Author: Gilberto Ribeiro de Queiroz
#
#  Description: Find LibGD include directory and library.
#
#  LIBGD_INCLUDE_DIR    -> where to find gd.h and other header files.
#  LIBGD_LIBRARY        -> LibGD library to link to.
#  LIBGD_FOUND          -> True if LibGD is found.
#

find_path(LIBGD_INCLUDE_DIR gd.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        libgd/include)

find_library(LIBGD_LIBRARY
             NAMES gd
             PATHS /usr
                   /usr/local
             PATH_SUFFIXES lib
                           libgd/lib)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibGD DEFAULT_MSG LIBGD_LIBRARY LIBGD_INCLUDE_DIR)

mark_as_advanced(LIBGD_INCLUDE_DIR LIBGD_LIBRARY)
