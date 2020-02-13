set(allowableCompilerVersion 7.2.1 8.2.1 8.3.1 9.2.1)

if(NOT COMPILER_VERSION_REQUIRED)
    if(NOT "${PROJECT_NAME}" STREQUAL "CMAKE_TRY_COMPILE")
        message(STATUS "No specific version required. ")
        message(AUTHOR_WARNING "please set variable COMPILER_WANTED_VERSION with one of this choice: ${allowableCompilerVersion}")

        #try to get the last item in the list
        list(LENGTH allowableCompilerVersion outLength)
        math(EXPR lastIndex "${outLength}-1")
        list(GET allowableCompilerVersion ${lastIndex} latestVersion)
        #---
        SET(COMPILER_WANTED_VERSION "${latestVersion}")
    else()
        set(Skip_Download ON)
    endif()
elseif(COMPILER_VERSION_REQUIRED IN_LIST allowableCompilerVersion)
    SET(COMPILER_WANTED_VERSION "${COMPILER_VERSION_REQUIRED}")
else()
    message(FATAL_ERROR "This version of ${TOOLCHAIN_PREFIX}gcc:${COMPILER_WANTED_VERSION} is not currently supported.")
endif()

if(NOT DEFINED Skip_Download)
    #set some variable
    if(7.2.1 VERSION_EQUAL COMPILER_WANTED_VERSION)
        set(COMPILER_WEB_BUILD_FOLDER 7-2017q4)
        set(COMPILER_RELEASE_TYPE "major")
        SET(COMPILER_MD5_APPLE 1EC5BED45D78788723036F22C4E83BA8)
        SET(COMPILER_MD5_WIN32 168C68C41EE0986ECC1DADCEAA8B6A3F)
        SET(COMPILER_MD5_UNIX  D3B00AE09E847747EF11316A8B04989A)
    elseif(8.2.1 VERSION_EQUAL COMPILER_WANTED_VERSION)
        set(COMPILER_WEB_BUILD_FOLDER 8-2018q4)
        set(COMPILER_RELEASE_TYPE "major")
        SET(COMPILER_MD5_APPLE 4c0d86df0244df22bc783f83df886db9)
        SET(COMPILER_MD5_WIN32 9b1cfb7539af11b0badfaa960679ea6f)
        SET(COMPILER_MD5_UNIX  f55f90d483ddb3bcf4dae5882c2094cd)
    elseif(8.3.1 VERSION_EQUAL COMPILER_WANTED_VERSION)
        set(COMPILER_WEB_BUILD_FOLDER 8-2019q3)
        set(COMPILER_RELEASE_TYPE "update")
        set(COMPILER_WEB_REVISION "/RC1.1")
        SET(COMPILER_MD5_APPLE 405cfbe54cee25a1b925ad0657f73924)
        SET(COMPILER_MD5_WIN32 a741af7570ae4a56e86e551652e6c365)
        SET(COMPILER_MD5_UNIX  6341f11972dac8de185646d0fbd73bfc)
    elseif(9.2.1 VERSION_EQUAL COMPILER_WANTED_VERSION)
        set(COMPILER_WEB_BUILD_FOLDER 9-2019q4)
        set(COMPILER_RELEASE_TYPE "major")
        set(COMPILER_WEB_REVISION "/RC2.1")
        SET(COMPILER_MD5_APPLE 241b64f0578db2cf146034fc5bcee3d4)
        SET(COMPILER_MD5_WIN32 82525522fefbde0b7811263ee8172b10)
        SET(COMPILER_MD5_UNIX  fe0029de4f4ec43cf7008944e34ff8cc)
    endif()

    string(REGEX REPLACE "(^[0-9]*-[0-9]*)" "\\1-" COMPILER_BUILD_NAME  ${COMPILER_WEB_BUILD_FOLDER})
    SET(COMPILER_EXTRACTED_RESULT_FOLDER "gcc-${TOOLCHAIN_PREFIX}${COMPILER_BUILD_NAME}-${COMPILER_RELEASE_TYPE}")

    if(APPLE)
        SET(COMPILER_FILE_NAME "${COMPILER_EXTRACTED_RESULT_FOLDER}-mac.tar.bz2")
        SET(COMPILER_MD5 ${COMPILER_MD5_APPLE})
        SET(COMPILER_BASE_PATH "${CMAKE_SOURCE_DIR}/Tools/Compiler")
        SET(MY_COMPILER_PATH "${COMPILER_BASE_PATH}/${COMPILER_WANTED_VERSION}/${COMPILER_EXTRACTED_RESULT_FOLDER}/bin")
    elseif(WIN32)
        SET(COMPILER_FILE_NAME "${COMPILER_EXTRACTED_RESULT_FOLDER}-win32.zip")
        SET(COMPILER_MD5 ${COMPILER_MD5_WIN32})
        SET(COMPILER_BASE_PATH "$ENV{APPDATA}/Tools/Compiler")
        SET(MY_COMPILER_PATH "${COMPILER_BASE_PATH}/${COMPILER_WANTED_VERSION}/bin")
    elseif(UNIX)
        SET(COMPILER_FILE_NAME "${COMPILER_EXTRACTED_RESULT_FOLDER}-linux.tar.bz2")
        SET(COMPILER_MD5 ${COMPILER_MD5_UNIX})
        SET(COMPILER_BASE_PATH "$ENV{HOME}/Tools/Compiler")
        SET(MY_COMPILER_PATH "${COMPILER_BASE_PATH}/${COMPILER_WANTED_VERSION}/${COMPILER_EXTRACTED_RESULT_FOLDER}/bin")
    endif()

    if(APPLE OR UNIX)
        SET( ENV{PATH} "${MY_COMPILER_PATH}:$ENV{PATH}" )
    elseif (WIN32)
        SET( ENV{PATH} "${MY_COMPILER_PATH};$ENV{PATH}" )
    endif()
endif()

if(MINGW OR CYGWIN OR WIN32)
    set(UTIL_SEARCH_CMD where)
    message(STATUS "search command where")
elseif(UNIX OR APPLE)
    set(UTIL_SEARCH_CMD which)
    message(STATUS "search command which")
endif()
#---
# Check if the gcc it can find the binUtils and if the version is correct
execute_process(
        COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
        OUTPUT_VARIABLE BINUTILS_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND ${TOOLCHAIN_PREFIX}gcc -dumpversion
        OUTPUT_VARIABLE MY_COMPILER_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

MESSAGE(STATUS "BinUtilsPath = " ${BINUTILS_PATH})
MESSAGE(STATUS "Wanted compiler version ${COMPILER_WANTED_VERSION} and found version = " ${MY_COMPILER_VERSION})
#---
if(NOT DEFINED  Skip_Download)
    if(BINUTILS_PATH STREQUAL "" OR NOT ${MY_COMPILER_VERSION} VERSION_EQUAL  ${COMPILER_WANTED_VERSION})
        message(STATUS "Compiler not found. downloading the correct version")
        SET(GCC_URL "https://developer.arm.com/-/media/Files/downloads/gnu-rm/${COMPILER_WEB_BUILD_FOLDER}${COMPILER_WEB_REVISION}/${COMPILER_FILE_NAME}")
        if((WIN32) AND (COMPILER_WANTED_VERSION VERSION_GREATER 8.3.1))
            SET(GCC_URL "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/${COMPILER_WEB_BUILD_FOLDER}${COMPILER_WEB_REVISION}/${COMPILER_FILE_NAME}")
        endif()
        message(STATUS "${GCC_URL}")
        file(DOWNLOAD ${GCC_URL}
                "${CMAKE_SOURCE_DIR}/${COMPILER_FILE_NAME}"
                EXPECTED_MD5;${COMPILER_MD5}
                SHOW_PROGRESS
                STATUS status)
        message(STATUS "Creating folders...")

        file(MAKE_DIRECTORY ${COMPILER_BASE_PATH}/${COMPILER_WANTED_VERSION})

        message(STATUS "Extracting the compiler")
        execute_process(
                COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/${COMPILER_FILE_NAME}
                WORKING_DIRECTORY ${COMPILER_BASE_PATH}/${COMPILER_WANTED_VERSION}
        )
        file(REMOVE ${CMAKE_SOURCE_DIR}/${COMPILER_FILE_NAME})
    else()
        message(STATUS "Compiler found with correct version. No need to install a new one")
    endif()
endif()
