file(GLOB EOWS_SRC_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/scidb/*.cpp)
file(GLOB EOWS_HDR_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/scidb/*.hpp)

source_group("Source Files"  FILES ${EOWS_SRC_FILES})
source_group("Header Files"  FILES ${EOWS_HDR_FILES})

if(APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
endif()

add_library(eows_scidb SHARED ${EOWS_HDR_FILES} ${EOWS_SRC_FILES})

target_link_libraries(eows_scidb   eows_core
                                   ${Boost_SYSTEM_LIBRARY}
                                   ${Boost_DATE_TIME_LIBRARY}
                                   ${Boost_CHRONO_LIBRARY}
                                   ${Boost_TIMER_LIBRARY}
                                   ${Boost_FILESYSTEM_LIBRARY}
                                   ${Boost_THREAD_LIBRARY}
                                   ${Boost_LOG_LIBRARY})

if(NOT APPLE)
  target_link_libraries(eows_scidb ${SCIDB_CLIENT_LIBRARY})
endif()

set_target_properties(eows_scidb
                      PROPERTIES VERSION ${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR}
                                 SOVERSION ${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR})

install(TARGETS eows_scidb
        EXPORT eows-targets
        RUNTIME DESTINATION ${EOWS_INSTALL_BIN_DIR} COMPONENT runtime
        LIBRARY DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT runtime
        ARCHIVE DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT devel)

install(FILES ${EOWS_HDR_FILES}
        DESTINATION "${EOWS_INSTALL_INCLUDE_DIR}/scidb"
        COMPONENT devel)

message(STATUS "EOWS: build of scidb module is enabled!")
