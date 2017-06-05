#
# PheatherStation
#
# This is the makefile for all of the subdirectories. It walks through
# and builds each module, in turn.
#
# Author:
#     Eric D. Schmidt <e1d1s1@hotmail.com> 06-05-2017
#

#
# All paths can include from 'lib' and this directory downward.
#

include_directories (
                      lib
                      .
                    )

#
# Add the major categories in.
#

add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(include)
#add_subdirectory(utilities)
