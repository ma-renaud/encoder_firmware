project(encoder_firmware_test C CXX)
cmake_minimum_required(VERSION 3.5.0)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
        gtest
        URL https://github.com/google/googletest/archive/v1.10.x.zip
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        CMAKE_ARGS -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
        # Disable install step
        INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/lib/libgmock.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
        "${source_dir}/googlemock/include"
        "../include")

message(STATUS "source_dir='${source_dir}'")

set(CMAKE_CXX_STANDARD 14)

file(GLOB INCLUDE_FILES "../include/*.h")

include_directories("include")
include_directories("system/include")
include_directories("system/include/cmsis")
include_directories("system/include/stm32f0xx")

file(GLOB_RECURSE SOURCE_FILES "../src/simpleProtocolParser.cpp"
        "../src/serialParser.cpp"
        "../src/serialParserStates.cpp"
        "../src/f4GpioMemory.cpp"
        "src/*.cpp")

include_directories("../include")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(TestingWithGTest ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(TestingWithGTest Threads::Threads libgtest libgmock)