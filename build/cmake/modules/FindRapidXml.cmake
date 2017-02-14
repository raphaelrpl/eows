#
#  Author: Gilberto Ribeiro de Queiroz
#
#  Description: Find RapidXml include directory.
#
#  RAPIDXML_INCLUDE_DIR - where to find rapidxml/rapidxml.hpp and other header files.
#  RAPIDXML_FOUND       - True if RapidXml found.
#

find_path(RAPIDXML_INCLUDE_DIR rapidxml/rapidxml.hpp
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        include/rapidxml)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(RapidXml DEFAULT_MSG RAPIDXML_INCLUDE_DIR)

mark_as_advanced(RAPIDXML_INCLUDE_DIR)
