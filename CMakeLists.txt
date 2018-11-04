message(STATUS "CMAKE_TOOLCHAIN_FILE='${CMAKE_TOOLCHAIN_FILE}'")

project(simple_blink CXX C ASM)
cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_CXX_STANDARD 14)

add_definitions(-DSTM32F401xE)

file(GLOB_RECURSE USER_SOURCES "src/*.c"
                               "src/*.cpp"
                               "system/src/cortexm/*.c"
                               "system/src/diag/*.c"
                               "system/src/newlib/*.c"
                               "system/src/newlib/*.cpp"
                               "system/src/stm32f4xx/*.c"
                               "system/src/cmsis/system_stm32f4xx.c"
                               "system/src/cmsis/startup_stm32f401xe.S")

file(GLOB_RECURSE HAL_SOURCES "system/stm32F4xx/*.c")

file(GLOB LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/ldscripts/mem.ld"
                        "${CMAKE_SOURCE_DIR}/ldscripts/libs.ld"
                        "${CMAKE_SOURCE_DIR}/ldscripts/sections.ld")

addLinkerScripts("${LINKER_SCRIPT}")

include_directories("include")
include_directories("system/include")
include_directories("system/include/cmsis")
include_directories("system/include/stm32f4xx")

add_executable(${PROJECT_NAME}.elf ${USER_SOURCES} ${HAL_SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${ARM_NONE_EABI_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${ARM_NONE_EABI_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
                   COMMENT "Invoking: Cross ARM GNU Print Size"
                   COMMAND ${ARM_NONE_EABI_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>)