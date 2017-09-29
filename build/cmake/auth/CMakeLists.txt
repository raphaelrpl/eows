include_directories(${JWTXX_INCLUDE_DIR})

file(GLOB_RECURSE EOWS_AUTH_SRC_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/auth/*.cpp)
file(GLOB_RECURSE EOWS_AUTH_HDR_FILES ${EOWS_ABSOLUTE_ROOT_DIR}/src/eows/auth/*.hpp)

source_group("Source Files"  FILES ${EOWS_AUTH_SRC_FILES})
source_group("Header Files"  FILES ${EOWS_AUTH_HDR_FILES})
add_library(eows_auth SHARED ${EOWS_AUTH_HDR_FILES}
                             ${EOWS_AUTH_SRC_FILES})

target_link_libraries(eows_auth eows_core
                                ${Boost_SYSTEM_LIBRARY}
                                ${Boost_DATE_TIME_LIBRARY}
                                ${Boost_CHRONO_LIBRARY}
                                ${Boost_TIMER_LIBRARY}
                                ${Boost_FILESYSTEM_LIBRARY}
                                ${Boost_THREAD_LIBRARY}
                                ${Boost_LOG_LIBRARY}
                                ${Boost_LOG_SETUP_LIBRARY}
                                ${CMAKE_THREAD_LIBS_INIT}
                                ${JANSSON_LIBRARIES}
                                ${JWTXX_LIBRARY})

set_target_properties(eows_auth  PROPERTIES
                                 VERSION ${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR}
                                 SOVERSION ${EOWS_VERSION_MAJOR}.${EOWS_VERSION_MINOR})

install(TARGETS eows_auth
        EXPORT eows-targets
        RUNTIME DESTINATION ${EOWS_INSTALL_BIN_DIR} COMPONENT runtime
        LIBRARY DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT runtime
        ARCHIVE DESTINATION ${EOWS_INSTALL_LIB_DIR} COMPONENT devel)

install(FILES ${EOWS_AUTH_HDR_FILES}
        DESTINATION "${EOWS_INSTALL_INCLUDE_DIR}/auth"
        COMPONENT devel)

message(STATUS "EOWS: build of authentication library is enabled!")
