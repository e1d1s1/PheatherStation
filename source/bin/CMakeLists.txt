#
# PheatherStation
#
# This is the makefile for the PheatherStation executable
#
# Author:
#     Eric D. Schmidt <e1d1s1@hotmail.com> 06-05-2017
#


#
# Record sources for the module.
#

set(SOURCE_FILES    "pheatherstation.cc")

#
# Record headers for the module.
#

set(HEADER_FILES "pheatherstation.hh")

#
# Create the shared library here.
#

add_executable(pheatherstation ${SOURCE_FILES} ${HEADER_FILES})

#
# Link against our dependencies.
#

target_link_libraries(pheatherstation)
