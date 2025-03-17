# Invoke the necessary macros to build an emcore application. Not to be
# confused with building an actual emcore. It is just the application which is
# linked to an emcore.
# \param: target -> this is the name of the executable target.
# \param: emcore -> must be one of EMCORE_FLAVORS with optional version E.g., "emb_ble_ll_hs_52;v1.0.0"
# \param: source_files -> list of source files to be compiled
# \param: include_files -> list of header files to be included
# \param: flags -> list of compile flags to be added. Use "" for no flags.
# \param(optional): libs -> list of libraries to be linked to the application.
# should not be one of 'EMCORE_LIBS_*'
#
# Example:
# GENERATE_EMCORE_APPLICATION(my_target emb_ble_ll_hs_52
#     "${${PROJECT_NAME}_SRCS}" "${${PROJECT_NAME}_INCLUDES}"
#     "${EMB_BLE_52_CONFIG}" "my_custom_lib"
# )
IF(NOT DEFINED EMCORE_VERSION)
    GIT_DESCRIBE("sdk-emb" EMCORE_VERSION "${ROOT_DIR}")
    MESSAGE("EMCore Version Not Defined, using EMB SDK version: ${EMCORE_VERSION}")
ENDIF()
MESSAGE("Add EM-Core libraries")
ADD_SUBDIRECTORY(libs)

FUNCTION(GENERATE_EMCORE_APPLICATION
        target emcore source_files include_files flags)
    SET(_tgt ${target})
    LIST(LENGTH emcore len_emcore)
    LIST(GET emcore 0 emcore_flavor)
    IF(${len_emcore} EQUAL 2) # Version included
        LIST(GET emcore 1 emcore_version)
        SET(EMC_VERSION ${ARGN})
        APP_USING_EMCORE("${emcore_flavor}" "${emcore_version}")
    ELSE()
        APP_USING_EMCORE("${emcore_flavor}")
    ENDIF()
    ARC_EXECUTABLE(${_tgt} ${source_files})
    ## optionally, libs to link can be included as the 5th argument
    IF (${ARGC} GREATER 5)
        ARC_LINK_LIBRARIES(${_tgt} ${ARGV5})
    ENDIF()

    ARC_INCLUDE_DIRECTORIES(${_tgt} ${include_files})
    ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${_tgt} ${flags})
ENDFUNCTION()

IF(NOT DEFINED EMCORE_VERSION)
    IF(SDK_BUILD OR EMCORE_DEV STREQUAL "false")
        MESSAGE("EMCORE Version Not Specified... looking for latest compiled version:")
        IF(SDK_BUILD)
            # This file is generated during the build process of the SDK
            INCLUDE("emcore.cmake")
        ENDIF()
        FILE(GLOB EMCORE_VERSION_LIST
             RELATIVE ${ROOT_DIR}/emcore/bin
             LIST_DIRECTORIES true
             ${ROOT_DIR}/emcore/bin/v*)
        FOREACH(emcv ${EMCORE_VERSION_LIST})
            MESSAGE("    EMCORE Versions Found: ${emcv}")
        ENDFOREACH()
        LIST(GET EMCORE_VERSION_LIST -1 __EMCORE_VERSION)
        # Using PARENT_SCOPE to make visible EMCORE_VERSION accorss CMake structure.
        SET(EMCORE_VERSION ${__EMCORE_VERSION} PARENT_SCOPE)
        SET(EMCORE_VERSION ${__EMCORE_VERSION})
        MESSAGE("    Using Latest EMCore binary version (${EMCORE_VERSION})")
    ENDIF()
ELSEIF(EMCORE_DEV STREQUAL "true")
    MESSAGE("ADDING emcore/dev directory to CMake structure")
    ADD_SUBDIRECTORY(dev)
ELSE()
    MESSAGE("EMCore Version is set to ${EMCORE_VERSION}")
ENDIF()

add_subdirectory(custom)
