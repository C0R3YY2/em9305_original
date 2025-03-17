################################################################################
# T9305 SDK                                                                    #
################################################################################

PROJECT(t9305_sdk C)

# Enable assembly.
SET(CMAKE_ASM_COMPILER_ID GNU)
ENABLE_LANGUAGE(ASM)

# Enable Firmware CRC Check (Option available since SDK 2.1)
SET(IMAGE_CRC_CHECK TRUE)

# Print the version of CMake being used.
CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
MESSAGE("-- Using CMake version ${CMAKE_VERSION}")

# Set the root directory of the EMB Extensions directory
SET(EMB_EXTENSIONS_DIR "${PROJECT_SOURCE_DIR}/libs/third_party/emb/emb_extensions")
# Print the version of Metaware being used
execute_process(COMMAND ${CMAKE_ASM_COMPILER} --version
                    RESULT_VARIABLE CCAC_VERSION_RESULT
                    OUTPUT_VARIABLE CCAC_VERSION_OUTPUT)

IF(${CCAC_VERSION_OUTPUT} MATCHES ".*(EM-Micro).*")
    SET(MW_FOR_EM TRUE)
ENDIF()

IF(${CCAC_VERSION_OUTPUT} MATCHES ".*2022\.09.*")
    SET(MW_VERSION "2022.09")
ELSEIF(${CCAC_VERSION_OUTPUT} MATCHES ".*2019\.09.*")
    SET(MW_VERSION "2019.09")
ENDIF()

IF(MW_FOR_EM)
    MESSAGE("-- Using Metaware version ${MW_VERSION} for EM-Micro")
ELSEIF(DEFINED MW_VERSION)
    MESSAGE("-- Using Metaware version ${MW_VERSION}")
ELSE()
    MESSAGE(WARNING "The Metaware Library associated to the installed Metaware version is not availabled in this SDK.\nBy default, the 2019.09 version is used")
    SET(MW_VERSION "2019.09")
ENDIF()

# Build options
OPTION(GENERATE_ASSEMBLY_DUMP
    "Generate an assembly dump for all libraries and executables." ON)
OPTION(INLINE_CODE_IN_ASSEMBLY_DUMP
    "Inline code in the assembly dump for all libraries and executables." OFF)

# Enable SDK build as we are in the SDK.
SET(SDK_BUILD   ON)

# Include the project constants and the other CMake files.
SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/t9305.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/t9305_design_iterations.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/emcore/bin/emcore_def.cmake OPTIONAL)
# Look for bin versions of emcore

FILE(GLOB EMCORE_BIN_LIST
        RELATIVE ${ROOT_DIR}/emcore/bin
        LIST_DIRECTORIES true
        ${ROOT_DIR}/emcore/bin/v*)

LIST(GET EMCORE_BIN_LIST 0 EMCORE_VERSION)

# Optional Include for custom_emcore definition. Must be defined prior building the libraries.
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/emcore/custom/custom_emcore_def.cmake OPTIONAL)
# Includes other build folders.
ADD_SUBDIRECTORY(libs)
ADD_SUBDIRECTORY(emcore)
ADD_SUBDIRECTORY(projects)
