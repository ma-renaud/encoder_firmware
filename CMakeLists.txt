cmake_minimum_required(VERSION 3.14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#Use a specific compiler version here
set(COMPILER_VERSION_REQUIRED "8.2.1" CACHE STRING "Use the fixed compiler version" FORCE)
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi-gcc.cmake" CACHE UNINITIALIZED "toolchain file")
endif ()

project(encoder_firmware CXX C ASM)

include(STM32F401xE)

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

string(REPLACE " " ";" STM_COMMON_COMPILE_FLAGS ${STM_COMMON_COMPILE_FLAGS})
set(PROJECT_COMMON_COMPILE_FLAGS "${STM_COMMON_COMPILE_FLAGS}" CACHE STRING "" FORCE)

add_executable(${PROJECT_NAME} ${USER_SOURCES} ${HAL_SOURCES} ${LINKER_SCRIPT})

target_compile_options(${PROJECT_NAME} PUBLIC
  ${PROJECT_COMMON_COMPILE_FLAGS}
  )

target_include_directories(${PROJECT_NAME}
  PRIVATE "include"
  PRIVATE "system/include"
  PRIVATE "system/include/cmsis"
  PRIVATE "system/include/stm32f4xx"
  )

target_link_libraries(${PROJECT_NAME}
  ${PROJECT_COMMON_COMPILE_FLAGS}
  -L"${CMAKE_SOURCE_DIR}/ldscripts"
  -T"libs.ld"
  -T"mem.ld"
  -T"sections.ld"
  --specs=nano.specs
  --specs=nosys.specs
  -Xlinker --gc-sections
  -Wl,-Map=${PROJECT_NAME}.map
  -Wl,--print-memory-usage
  )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_features(${PROJECT_NAME} PUBLIC c_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME}.elf -O binary ${PROJECT_NAME}.bin
  COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME}.elf -O ihex ${PROJECT_NAME}.hex
  COMMAND ${CMAKE_NM_UTIL} -C -r --size-sort -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.nm
  COMMAND ${CMAKE_SIZE_UTIL} --format=berkeley ${PROJECT_NAME}.elf
  )

#add_subdirectory(test)