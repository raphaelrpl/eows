# Configuring CMake global values
set(CMAKE_PREFIX_PATH "$ENV{EOWS_3RDPARTY};$ENV{EOWS_SCIDB_DIR};$ENV{EOWS_BOOST_DIR}" CACHE STRING "Path to Libraries" FORCE)

# Turning on EOWS flags
set(EOWS_GEOARRAY_ENABLED ON CACHE BOOL "Build GeoArray module?" FORCE)

