file(GLOB EOWS_SRC_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/ogc/ows/*.cpp)
file(GLOB EOWS_HDR_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/ogc/ows/*.hpp)

source_group("Source Files"  FILES ${EOWS_SRC_FILES})
source_group("Header Files"  FILES ${EOWS_HDR_FILES})

add_library(eows_ows SHARED ${EOWS_HDR_FILES} ${EOWS_SRC_FILES})

target_link_libraries(eows_ows    eows_core
                                  ${Boost_SYSTEM_LIBRARY}
                                  ${Boost_DATE_TIME_LIBRARY}
                                  ${Boost_CHRONO_LIBRARY}
                                  ${Boost_TIMER_LIBRARY}
                                  ${Boost_FILESYSTEM_LIBRARY}
                                  ${Boost_THREAD_LIBRARY}
                                  ${Boost_LOG_LIBRARY})

set_target_properties(eows_ows
                      PROPERTIES VERSION ${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR}
                                 SOVERSION ${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR})

install(TARGETS eows_ows
        EXPORT eows-targets
        RUNTIME DESTINATION ${EOWS_INSTALL_BIN_DIR} COMPONENT runtime
        LIBRARY DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT runtime
        ARCHIVE DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT devel)

install(FILES ${EOWS_HDR_FILES}
        DESTINATION "${EOWS_INSTALL_INCLUDE_DIR}/ogc/ows"
        COMPONENT devel)

message(STATUS "EOWS: build of OGC OWS is enabled!")
