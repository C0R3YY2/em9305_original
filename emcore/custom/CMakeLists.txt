PROJECT(custom_emcore C)
MESSAGE("ADDING emcore/custom directory to CMake structure")
# Set EMCore version
SET(CUSTOM_EMCORE_VERSION "${EMCORE_VERSION}")
# To identify the flavor of EMCore, an ID is added to the Firmware Header section in NVM.
# This ID shall be defined with ID 0x0F (15). Values from 0 to 14 are reserved.

SET(CUSTOM_EMCORE_BIN_DIR  "${EMCORE_ROOT_DIR}/bin/${CUSTOM_EMCORE_VERSION}")
SET(CUSTOM_EMCORE_SRC_DIR  ${EMCORE_ROOT_DIR}/custom)

MESSAGE("Custom EMCORE Version used for this build: ${CUSTOM_EMCORE_VERSION}")
MESSAGE("Custom EMCORE binaries are located in ${CUSTOM_EMCORE_BIN_DIR}")

MESSAGE("CUSTOM EMCORE Flavors:")
SET(EMCORE_FLAVORS "${EMCORE_FLAVORS}" "${CUSTOM_EMCORE_FLAVORS}")

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/custom_emcore.cmake")

FOREACH(custom_emcore_flavor ${CUSTOM_EMCORE_FLAVORS})
    GET_EMCORE_FLAVOR_INFO("${custom_emcore_flavor}" custom_emcore_flavor_name custom_emcore_flavor_id)
    IF(NOT ${custom_emcore_flavor_id} EQUAL -1) # Ignore invalid IDX
        MESSAGE("  ${custom_emcore_flavor_name} (ID ${custom_emcore_flavor_id})")
    ENDIF()
ENDFOREACH()

