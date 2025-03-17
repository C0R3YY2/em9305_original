################################################################################
# T9305 project constants and macros.                                          #
# This file shall be included from the root CMakeLists.txt file!               #
################################################################################

# Set the target hardware.
SET(TARGET_HW       9305)

# Set ROOT_DIR to its default value if not set earlier
# This allows some flexibility in the SDK location with  respect to libraries
# and project source code not being part of the SDK.
IF(NOT DEFINED ROOT_DIR)
    SET(ROOT_DIR    ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF()
MESSAGE("ROOT_DIR set to ${ROOT_DIR}")

# Set other useful directories.
SET(BUILD_DIR       ${CMAKE_BINARY_DIR})
SET(CMAKE_DIR       ${ROOT_DIR}/cmake)
SET(FPGA_DIR        ${ROOT_DIR}/fpga)
SET(ROM_DIR         ${ROOT_DIR}/rom)
SET(LIBS_DIR        ${ROOT_DIR}/libs)
SET(COMMON_DIR      ${ROOT_DIR}/common)
SET(SDK_DIR         ${ROOT_DIR}/sdk)
SET(DOWNLOAD_DIR    ${ROOT_DIR}/download)
SET(EMCORE_ROOT_DIR ${ROOT_DIR}/emcore)
SET(EMCORE_BIN_DIR  ${EMCORE_ROOT_DIR}/bin)
SET(EMCORE_SRC_DIR  ${EMCORE_ROOT_DIR}/dev)
SET(CUSTOM_EMCORE_SRC_DIR  ${EMCORE_ROOT_DIR}/custom)
SET(EMCORE_LIB_DIR  ${EMCORE_ROOT_DIR}/libs)

# Change the executable suffix for ARC.
SET(CMAKE_EXECUTABLE_SUFFIX     ".elf")

# Change the static library prefix.
SET(CMAKE_STATIC_LIBRARY_PREFIX "lib_")

# Address and size in bytes of the IROM.
SET(HW_IROM_ADDR            "0x00100000")
SET(HW_IROM_SIZE            65536)
SET(HW_IROM_PLUS_SIZE       262144)

# Address and size in bytes of the NVM main area.
SET(HW_NVM_ADDR             "0x00300000")
SET(HW_NVM_SIZE             524288)

# Address and size in bytes of the NVM info area.
SET(HW_NVM_INFO_ADDR        "0x00400000")
SET(HW_NVM_INFO_SIZE        32768)

# Address and size in bytes of the IRAM (for test code in IRAM).
SET(HW_IRAM_TESTCODE_ADDR   "0x00184000")
SET(HW_IRAM_TESTCODE_SIZE   16384)

# Size of the stack (in bytes).
SET(STACKSIZE       4096)

# ROM project constants.
SET(ROM_BASENAME_ASIC   "rom")
SET(ROM_POSTFIX_SPI     "")
SET(ROM_POSTFIX_UART    "_uart")

# Add the CMake directory to the include search directories.
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_DIR})

# These are the utilities used by CMake for building.
INCLUDE(CMakeParseArguments)
INCLUDE(${CMAKE_DIR}/macros/macros_build_configs.cmake)
INCLUDE(${CMAKE_DIR}/macros/macros_flags.cmake)
INCLUDE(${CMAKE_DIR}/macros/macros_nvm_multi_apps.cmake OPTIONAL)
INCLUDE(${CMAKE_DIR}/macros/macros_targets_common.cmake)
INCLUDE(${CMAKE_DIR}/macros/macros_targets_executables.cmake)
INCLUDE(${CMAKE_DIR}/macros/macros_targets_libraries.cmake)
INCLUDE(${CMAKE_DIR}/macros/macros_versioning.cmake OPTIONAL)
INCLUDE(${CMAKE_DIR}/macros/macros_emcore.cmake)

INCLUDE(${CMAKE_DIR}/utils.cmake)
INCLUDE(${CMAKE_DIR}/build_configs.cmake)
INCLUDE(${CMAKE_DIR}/flags.cmake)
INCLUDE(${CMAKE_DIR}/common/common_includes.cmake)
INCLUDE(${CMAKE_DIR}/common/common_libs.cmake)
INCLUDE(${CMAKE_DIR}/ccache.cmake)
INCLUDE(${SDK_DIR}/cmake/macros_sdk.cmake OPTIONAL)
