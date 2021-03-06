SET(STM_COMMON_COMPILE_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra")

SET(CMAKE_C_FLAGS_DEBUG "${COMMON_FLAGS} -O0 -g3 " CACHE INTERNAL "c compiler flags debug")
SET(CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS} -O0 -g3 -fno-exceptions -fno-rtti -fno-use-cxa-atexit " CACHE INTERNAL "cxx compiler flags debug")

SET(CMAKE_C_FLAGS_RELEASE "${COMMON_FLAGS} -O3 -g" CACHE INTERNAL "c compiler flags release")
SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS} -O3 -g -fno-exceptions -fno-rtti -fno-use-cxa-atexit " CACHE INTERNAL "cxx compiler flags release")

add_definitions(-DSTM32F401xE)