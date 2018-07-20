#
# PheatherStation
#
# This is the makefile for pheatherstation firmware.
#
# Author:
#     Eric D. Schmidt <e1d1s1@hotmail.com> 06-05-2017
#

#
# we want to be using C++14 support.
#

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
	pheatherstation.cpp
	humidity.cpp
	web_handler.cpp
)

set(HEADER_FILES
	${BASE_DIRECTORY}/include/humidity.h
	${BASE_DIRECTORY}/include/string_helper.h
	${BASE_DIRECTORY}/include/web_handler.h
	${BASE_DIRECTORY}/include/pheatherstation.h
	${BASE_DIRECTORY}/include/eeprom_settings.h
	${BASE_DIRECTORY}/platform-particle/cactus_io_BME280_I2C.h
)

set(PLATFORM_SOURCE
	${BASE_DIRECTORY}/platform-particle/main.cpp
	${BASE_DIRECTORY}/platform-particle/cactus_io_BME280_I2C.cpp
)

include_directories(
        ${BASE_DIRECTORY}/libs/firmware/system/inc
        ${BASE_DIRECTORY}/libs/firmware/hal/inc
        ${BASE_DIRECTORY}/libs/firmware/hal/shared
        ${BASE_DIRECTORY}/libs/firmware/hal/src/stm32f2xx
        ${BASE_DIRECTORY}/libs/firmware/services/inc
        ${BASE_DIRECTORY}/libs/firmware/wiring/inc
        ${BASE_DIRECTORY}/libs/firmware/user/inc
	${BASE_DIRECTORY}/libs/firmware/communication/src
	${BASE_DIRECTORY}/libs/firmware/hal/src/photon/wiced/platform/MCU/STM32F2xx/peripherals/libraries
	${BASE_DIRECTORY}/libs/firmware/platform/MCU/STM32F2xx/CMSIS/Include
	${BASE_DIRECTORY}/libs/firmware/hal/src/photon/wiced/platform/MCU/STM32F2xx/peripherals/libraries/inc
	${BASE_DIRECTORY}/libs/firmware/hal/src/photon/wiced/platform/ARM_CM3
	${BASE_DIRECTORY}/libs/firmware/platform/MCU/gcc/inc
	${BASE_DIRECTORY}/libs/firmware/hal/src/stm32
        )

add_library(pheatherstation-core STATIC EXCLUDE_FROM_ALL ${HEADER_FILES} ${SOURCE_FILES})
add_executable(pheatherstation-exe EXCLUDE_FROM_ALL ${PLATFORM_SOURCE})
target_include_directories(pheatherstation-core PRIVATE ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(pheatherstation-core)
target_include_directories(pheatherstation-core PUBLIC ${CMAKE_SOURCE_DIR}/include)
