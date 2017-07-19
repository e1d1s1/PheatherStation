#
#
# This file retrieves the current git version and tags the repository
# with it, to improve traceability in our binaries.
#

#
# Ask git for the current changeset (and date).
#

execute_process (
                  COMMAND git log --pretty=format:"%H %ct" -n 1
                  WORKING_DIRECTORY .
                  OUTPUT_VARIABLE GIT_PRETTYCHANGESET
                )

#
# Ask git for the current changeset (and date), no quotes
#

execute_process (
                  COMMAND git log --pretty=format:%H -n 1
                  WORKING_DIRECTORY .
                  OUTPUT_VARIABLE GIT_CHANGESET
                )

execute_process (
                  COMMAND git log --pretty=format:%ct -n 1
                  WORKING_DIRECTORY .
                  OUTPUT_VARIABLE GIT_DATE
                )
#
# Get the current git tag
#
execute_process (
                  COMMAND git describe --tags
                  WORKING_DIRECTORY .
                  OUTPUT_VARIABLE GIT_TAG
                )

execute_process (
                  COMMAND git describe --tags
                  WORKING_DIRECTORY .
                  OUTPUT_VARIABLE GIT_PRETTYTAG
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                )

#
# Get the current branch
#
execute_process (
                  COMMAND git rev-parse --abbrev-ref HEAD
                  WORKING_DIRECTORY .
                  OUTPUT_VARIABLE GIT_BRANCH
                )
            
#
# Get the git status as well, so we know if there is uncommitted
# files present.
#

execute_process (
                  COMMAND git status
                  WORKING_DIRECTORY .
                  OUTPUT_VARIABLE GIT_STATUS
                )
                
#
# And then grab the full log entry.
#

execute_process (
                  COMMAND git log -n 1
                  WORKING_DIRECTORY .
                  OUTPUT_VARIABLE GIT_LOG
                )

#
# Reform the changeset into a #define.
#

# deal with tagless repo
if ("${GIT_PRETTYTAG}" STREQUAL "") 
    set(GIT_PRETTYTAG "NOTAG")
endif()

set(DESIRED_CHANGESET_HEADER "#define REVISION_CHANGESET ${GIT_PRETTYCHANGESET}")
set(DESIRED_VERSION_HEADER "#define REVISION_TAG \"${GIT_PRETTYTAG}\"")

#
# Look for the output header.
#

set(OUTPUT_HEADER "${BASE_DIRECTORY}/changeset.h")
                
#
# Before we write out anything, let's ensure that it has changed. Read
# in the current header.
#

if(EXISTS ${OUTPUT_HEADER})

    file(READ ${OUTPUT_HEADER} PRESENT_CHANGESET_HEADER LIMIT 1024)
    
else()

    set(PRESENT_CHANGESET_HEADER "")
    
endif()

#
# If the two files do not match, then overwrite the existing file.
#

if(NOT PRESENT_CHANGESET_HEADER STREQUAL DESIRED_CHANGESET_HEADER)

    message("-- New changeset was pushed, updating changeset information in the pheatherstation binary.")
    
    file(WRITE ${OUTPUT_HEADER} "${DESIRED_CHANGESET_HEADER}")
    file(APPEND ${OUTPUT_HEADER} "\n${DESIRED_VERSION_HEADER}")
    
    #
    # Additionally, write the changeset information out to a file in the
    # configuration directory, so it is compressed and trackable.
    #
 
    set(OUTPUT_CONFIG "${BASE_DIRECTORY}/output/repository_status.txt")
    
    file(WRITE ${OUTPUT_CONFIG} "${GIT_LOG}\n${GIT_STATUS}")    

endif()
