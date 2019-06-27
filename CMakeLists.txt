# CMake project file for fislator
cmake_minimum_required (VERSION 2.8)

# project name and details
project (FISLATOR)
set(PROJECT_DESCRIPTION "FISPACT-II JSON file translator")
set(PROJECT_URL "https://github.com/fispact/fislator")

# set source directories
set (SOURCE_DIR_BASE ${CMAKE_CURRENT_LIST_DIR}/include)
set (THIRD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/dependencies)
set (TOOLS_DIR ${CMAKE_CURRENT_LIST_DIR}/tools)
set (FISLATOR_BUILD_DIR ${CMAKE_BINARY_DIR})

# Get the fislator version, set in .VERSION file
file ( STRINGS "${CMAKE_SOURCE_DIR}/.VERSION" VERSION )
string( REPLACE "." ";" VERSION_LIST ${VERSION} )
list(GET VERSION_LIST 0 VERSION_MAJOR)
list(GET VERSION_LIST 1 VERSION_MINOR)
list(GET VERSION_LIST 2 VERSION_PATCH)
set(FISLATOR_PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message ( STATUS "CMake build configuration for FISLATOR ${FISLATOR_PROJECT_VERSION}" )

# C++ flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# set the output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# CLI is header only
set (CLI_INCLUDES ${THIRD_PARTY_DIR}/CLI11/include)

# json is header only
set (JSON_INCLUDES ${THIRD_PARTY_DIR}/json/include)

# set includes
include_directories(${CLI_INCLUDES}
                    ${JSON_INCLUDES}
                    ${SOURCE_DIR_BASE})

# sources
add_subdirectory(${TOOLS_DIR})
