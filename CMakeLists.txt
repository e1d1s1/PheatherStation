#
# PheatherStation
#
# This is the master makefile for the entire project.
#
# Author:
#     Eric D. Schmidt <e1d1s1@hotmail.com> 06-05-2017
#     Used https://github.com/ashtuchkin/vive-diy-position-sensor as a guide

#
# Make sure there is a reasonable version of CMAKE installed.
#

cmake_minimum_required(VERSION 3.2)

#
# Keep track of the base directory for the rest of the system.
#

set(BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#cross compiler stuff
#if (${PLATFORM} MATCHES "Particle")
	set(PLATFORM "Particle")
    set(CMAKE_TOOLCHAIN_FILE "platform-particle/stm32f2xx.toolchain.cmake")
    add_subdirectory(platform-particle)
#endif()

#
# Define the base solution name.
#

project("PheatherStaton")

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
     "Reset the configurations to what we need"
     FORCE)
endif()

# weather calculations
add_subdirectory(src)

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
endif()

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
# Create a target to recalculate the source control hash.
#

add_custom_target (changeset
                    COMMAND ${CMAKE_COMMAND} -DBASE_DIRECTORY=${BASE_DIRECTORY} -P ${BASE_DIRECTORY}/cmake/GetChangeSetInfo.cmake
                    WORKING_DIRECTORY ${BASE_DIRECTORY}
                    VERBATIM
                  )

add_dependencies(pheatherstation-core changeset)

set_property(TARGET changeset PROPERTY FOLDER "CMakePredefinedTargets")
