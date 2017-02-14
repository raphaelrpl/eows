#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#
#  Description: Find Crow include directory.
#
#  CROW_INCLUDE_DIR - where to find crow.h or crow_all.h.
#  Crow_FOUND       - True if Crow is found.
#

find_path(CROW_INCLUDE_DIR NAMES crow.h crow_all.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        crow/include)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Crow DEFAULT_MSG CROW_INCLUDE_DIR)

mark_as_advanced(CROW_INCLUDE_DIR)
