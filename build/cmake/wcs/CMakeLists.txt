file(GLOB_RECURSE EOWS_SRC_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/ogc/wcs/*.cpp)
file(GLOB_RECURSE EOWS_HDR_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/ogc/wcs/*.hpp)

source_group("Source Files"  FILES ${EOWS_SRC_FILES})
source_group("Header Files"  FILES ${EOWS_HDR_FILES})

add_library(eows_wcs SHARED ${EOWS_HDR_FILES} ${EOWS_SRC_FILES})

target_link_libraries(eows_wcs    eows_core
                                  eows_geoarray
                                  eows_proj4
                                  eows_scidb
                                  eows_gdal2
                                  ${Boost_SYSTEM_LIBRARY}
                                  ${Boost_DATE_TIME_LIBRARY}
                                  ${Boost_CHRONO_LIBRARY}
                                  ${Boost_TIMER_LIBRARY}
                                  ${Boost_FILESYSTEM_LIBRARY}
                                  ${Boost_THREAD_LIBRARY}
                                  ${Boost_LOG_LIBRARY})

if(EOWS_AUTH_ENABLED)
  target_link_libraries(eows_wcs eows_auth)
endif()

set_target_properties(eows_wcs
                      PROPERTIES VERSION ${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR}
                                 SOVERSION ${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR})

install(TARGETS eows_wcs
        EXPORT eows-targets
        RUNTIME DESTINATION ${EOWS_INSTALL_BIN_DIR} COMPONENT runtime
        LIBRARY DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT runtime
        ARCHIVE DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT devel)

install(FILES ${EOWS_HDR_FILES}
        DESTINATION "${EOWS_INSTALL_INCLUDE_DIR}/ogc/wcs"
        COMPONENT devel)

message(STATUS "EOWS: build of OGC WCS is enabled!")
