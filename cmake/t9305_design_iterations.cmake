################################################################################
# T9305 HW Design Iterations.                                                  #
# This file shall be included from the root CMakeLists.txt file!               #
################################################################################

# List of HW Design Iterations.
SET(HW_DI_INDEX_FULL
    "dev"
    ""
    ""
    "di03"
    "di04"
    "di05"
)
SET(HW_DI_INDEX_SDK
    ""
    ""
    ""
    "di03"
    "di04"
    "di05"
)

IF(NOT DEFINED DEFAULT_DI)
    LIST(LENGTH HW_DI_INDEX_SDK di_list_length)
    MATH(EXPR last_idx_in_list "${di_list_length} -1")
    LIST(GET HW_DI_INDEX_SDK ${last_idx_in_list} DEFAULT_DI)
    MESSAGE("Default target set to most recent DI (${DEFAULT_DI})")
    SET(DEFAULT_DI ${DEFAULT_DI})
ELSE()
    IF(${DEFAULT_DI} IN_LIST HW_DI_INDEX_SDK)
        MESSAGE("Default target is built for set to ${DEFAULT_DI}")
    ELSE()
        MESSAGE(FATAL_ERROR "Default target specified is not supported (DEFAULT_DI = ${DEFAULT_DI} not recognized)")
    ENDIF()
ENDIF()

# Link between HW Design Iterations and ROM versions.
SET(ROM_VERSION_dev     "dev")
SET(ROM_VERSION_di01    "v1.0")
SET(ROM_VERSION_di03    "v1.0")
SET(ROM_VERSION_di04    "v2.0")
SET(ROM_VERSION_di05    "v3.0")

# Freeze ROM
# If the ROM version is not null the ROM version is freezed.
SET(PUBLISH_ROM $ENV{PUBLISH_ROM})
SET(ROM_VERSION_FREEZED $ENV{ROM_VERSION_FREEZED})
IF(NOT ${ROM_VERSION_FREEZED} STREQUAL "")
    MESSAGE("ROM VERSION IS FREEZED TO ${ROM_VERSION_FREEZED}")
ENDIF()

# Get the list of available HW Design Iterations.
SET(HW_DI_LIST_FULL "")
FOREACH(hw_di ${HW_DI_INDEX_FULL})
    IF(NOT "${hw_di}" STREQUAL "")
        LIST(APPEND HW_DI_LIST_FULL ${hw_di})
    ENDIF()
ENDFOREACH()
SET(HW_DI_LIST_SDK "")
FOREACH(hw_di ${HW_DI_INDEX_SDK})
    IF(NOT "${hw_di}" STREQUAL "")
        LIST(APPEND HW_DI_LIST_SDK ${hw_di})
    ENDIF()
ENDFOREACH()

# Select the correct HW Design Iterations list.
IF(ONLY_DEV)
    SET(HW_DI_INDEX "dev")
    SET(HW_DI_LIST  "dev")
ELSEIF(NOT SDK_BUILD)
    SET(HW_DI_INDEX "${HW_DI_INDEX_FULL}")
    SET(HW_DI_LIST  "${HW_DI_LIST_FULL}")
ELSE()
    SET(HW_DI_INDEX "${HW_DI_INDEX_SDK}")
    SET(HW_DI_LIST  "${HW_DI_LIST_SDK}")
ENDIF()

# Be sure there are some available HW Design Iterations.
LIST(LENGTH HW_DI_LIST hw_di_list_length)
IF(${hw_di_list_length} EQUAL 0)
    MESSAGE(FATAL_ERROR "No HW Design Iteration available!")
ENDIF()

# Print the available HW Design Iterations.
MESSAGE("HW Design Iterations:")
FOREACH(hw_di ${HW_DI_LIST})
    MESSAGE("  ${hw_di} (ROM ${ROM_VERSION_${hw_di}})")
ENDFOREACH()
