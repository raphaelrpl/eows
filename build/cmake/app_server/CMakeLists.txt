file(GLOB EOWS_SRC_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/app_server/*.cpp)
file(GLOB EOWS_HDR_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/app_server/*.hpp)

source_group("Source Files"  FILES ${EOWS_SRC_FILES})
source_group("Header Files"  FILES ${EOWS_HDR_FILES})

add_executable(eows_app_server ${EOWS_SRC_FILES} ${EOWS_HDR_FILES})

target_link_libraries(eows_app_server   eows_core
                                        ${Boost_SYSTEM_LIBRARY}
                                        ${Boost_DATE_TIME_LIBRARY}
                                        ${Boost_CHRONO_LIBRARY}
                                        ${Boost_TIMER_LIBRARY}
                                        ${Boost_FILESYSTEM_LIBRARY}
                                        ${Boost_THREAD_LIBRARY}
                                        ${Boost_LOG_LIBRARY}
                                        ${Boost_PROGRAM_OPTIONS_LIBRARY}
                                        ${CMAKE_THREAD_LIBS_INIT})

if(EOWS_PROJ4_ENABLED)
  target_link_libraries(eows_app_server eows_proj4)
  target_link_libraries(eows_app_server ${PROJ4_LIBRARY})
endif()

if(EOWS_GDAL2_ENABLED)
  target_link_libraries(eows_app_server eows_gdal2)
  target_link_libraries(eows_app_server ${GDAL_LIBRARY})
endif()

if(EOWS_GEOARRAY_ENABLED)
  target_link_libraries(eows_app_server eows_geoarray)
endif()

if(EOWS_GEOCACHE_ENABLED)
  target_link_libraries(eows_app_server eows_geocache)
endif()

if(EOWS_AUTH_ENABLED)
  target_link_libraries(eows_app_server eows_auth)
endif()

if(EOWS_OGC_OWS_ENABLED)
  target_link_libraries(eows_app_server eows_ows)
endif()

if(EOWS_SCIDB_ENABLED)
  target_link_libraries(eows_app_server eows_scidb ${SCIDB_CLIENT_LIBRARY})
endif()

if(EOWS_HTTP_CPPNETLIB_ENABLED)
  target_link_libraries(eows_app_server http_cppnetlib cppnetlib-server-parsers cppnetlib-uri)
endif()

if(EOWS_HTTP_CROW_ENABLED)
  target_link_libraries(eows_app_server http_crow)
endif()

if(EOWS_SERVICE_ECHO_ENABLED)
  target_link_libraries(eows_app_server eows_echo)
endif()

if(EOWS_SERVICE_WTSS_ENABLED)
  target_link_libraries(eows_app_server eows_wtss)
endif()

if(EOWS_SERVICE_WCS_ENABLED)
  target_link_libraries(eows_app_server eows_wcs)
endif()

if(EOWS_SERVICE_WMS_ENABLED)
  target_link_libraries(eows_app_server eows_ogcwms)
endif()

if(EOWS_SERVICE_WTSCS_ENABLED)
  target_link_libraries(eows_app_server eows_wtscs)
endif()

install(TARGETS eows_app_server
        EXPORT eows-targets
        RUNTIME DESTINATION ${EOWS_INSTALL_BIN_DIR} COMPONENT runtime
        LIBRARY DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT runtime
        ARCHIVE DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT devel)

install(FILES ${EOWS_HDR_FILES}
        DESTINATION "${EOWS_INSTALL_INCLUDE_DIR}/app_server"
        COMPONENT devel)

message(STATUS "EOWS: build of application server is enabled!")
