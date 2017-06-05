#
# PheatherStation
#
# This is the master makefile for the entire project.
#
# Author:
#     Eric D. Schmidt <e1d1s1@hotmail.com> 06-05-2017
#

#
# Make sure there is a reasonable version of CMAKE installed.
#

cmake_minimum_required(VERSION 2.6)

#
# Define the build type. We default to RelWithDebInfo
#
IF(DEFINED CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")   
ELSE()
   SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
ENDIF()

message("Build configuration type is ${CMAKE_BUILD_TYPE}")

#
# Define the base solution name.
#

project("PheatherStaton")

#
# Keep track of the base directory for the rest of the system.
#

set(BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#
# This is where code is released to. This directory forms the
# final "installation", allowing you to copy one directory to
# a remote machine to avoid publishing source code.
#

set(RELEASE_DIRECTORY ${BASE_DIRECTORY}/output)


#
# Install to the target directory.
#

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RELEASE_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RELEASE_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RELEASE_DIRECTORY})

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
     "Reset the configurations to what we need"
     FORCE)
endif()


#
# Avoid needing to set LD_LIBRARY_PATH (or install to /usr/lib).
# This tells our binaries to look into the same base directory for
# their libraries.
#

set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH false)
set(CMAKE_SKIP_BUILD_RPATH false)
set(CMAKE_SKIP_RPATH false)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/.:\$ORIGIN/..")

#
# we want to be using C++14 support.
#

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

#
# Make sure our dependencies are all present (as much as we
# care to check).
#

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${BASE_DIRECTORY}/cmake")


# unit test support
option(DOTESTS "Run unit tests" OFF)
if (DOTESTS)
	#unit test library
	find_package(Check REQUIRED)
    enable_testing()
    add_subdirectory (tests)
endif()

#
# Include directories for dependencies.
#

include_directories (
                      ${CMAKE_CURRENT_BINARY_DIR}/source/lib
                      ${CMAKE_CURRENT_SOURCE_DIR}/source/include
                      ${TOOLCHAIN_INCLUDE_DIRECTORIES}
                    )

#
# Coverage instrumentation
#

set(COVERAGE_TESTING OFF CACHE BOOL "Add coverage instrumentation into binaries.")

if (COVERAGE_TESTING)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

    set(COVERAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/coverage_result")
    set(COVERAGE_INDEX "${COVERAGE_DIR}/index.html")

    add_custom_target(cleancoverage)
    add_custom_command (
                         TARGET cleancoverage
                         POST_BUILD
                         COMMAND ${LCOV} --directory . --zerocounters
                         COMMAND test -d ${COVERAGE_DIR} && rm -r ${COVERAGE_DIR} || exit 0
                       )

    add_custom_command (
                         OUTPUT ${COVERAGE_INDEX}
                         COMMAND ${LCOV} -c -d . -o coverage.info
                         COMMAND ${LCOV} -r coverage.info /usr/include/\\* -o coverage.info
                         COMMAND ${GENHTML} -o ${COVERAGE_DIR} coverage.info
                       )

    add_custom_target(coverage DEPENDS ${COVERAGE_INDEX})

    add_custom_target(showcoverage DEPENDS coverage)
    add_custom_command (
                         TARGET showcoverage
                         POST_BUILD
                         COMMAND firefox ${COVERAGE_INDEX}
                       )
endif ()

#
# Alright, move on to building the main source folder now.
#

add_subdirectory(source)

#
# Create a target to recalculate the source control hash.
#

add_custom_target (changeset
                    COMMAND ${CMAKE_COMMAND} -DBASE_DIRECTORY=${BASE_DIRECTORY} -P ${BASE_DIRECTORY}/cmake/GetChangeSetInfo.cmake
                    WORKING_DIRECTORY ${BASE_DIRECTORY}
                    VERBATIM
                  )

add_dependencies(pheatherstation changeset)

set_property(TARGET changeset PROPERTY FOLDER "CMakePredefinedTargets")
