PROJECT(emcore_information C)
IF(NOT DEFINED EMCORE_FLAVORS)
    MESSAGE(
        FATAL_ERROR
        "${PROJECT_NAME} EMCORE_FLAVORS must be defined before this point in the cmake."
    )
ENDIF()

SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    includes
)

SET(${PROJECT_NAME}_SRCS
    source/emcore_information.c
)

IF(NOT DEFINED EMCORE_VERSION)
    GIT_DESCRIBE("sdk-emb" EMCORE_VERSION "${ROOT_DIR}")
    MESSAGE("EMCore Version Not Defined, using EMB SDK version: ${EMCORE_VERSION}")
ENDIF()
IF("${emcore_version_string}" STREQUAL "")
    SET(emcore_version_string "git_version_not_available.")
ENDIF()

SET(emcore_flavor_count 0)
FOREACH(emcore_flavor ${EMCORE_FLAVORS} ${CUSTOM_EMCORE_FLAVORS})
    GET_EMCORE_FLAVOR_INFO("${emcore_flavor}" emcore_flavor_name emcore_flavor_id)
    IF(NOT ${emcore_flavor_id} STREQUAL "-1")
        SET(tgt "${PROJECT_NAME}_${emcore_flavor_name}")
        # NVM Library For each emcore flavor
        LIB_IN_NVM()
        ARC_ADD_LIBRARY(${tgt} ${${PROJECT_NAME}_SRCS})
        ARC_INCLUDE_DIRECTORIES(${tgt} ${${PROJECT_NAME}_INCLUDES})
        # Todo: move this lib in EM-Core dev structure. It shall be built along with the EM-Core.
        ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${tgt}
            "-DEMCORE_FLAVOR=${emcore_flavor}"
            "-DEMCORE_VERSION_STR=${EMCORE_VERSION}"
            "-DEMCORE_FLAVOR_ID=${emcore_flavor_id}"
        )
    ENDIF()
ENDFOREACH()

# NVM Library for non-emcore things.
LIB_IN_NVM()
ARC_ADD_LIBRARY(${PROJECT_NAME}_default ${${PROJECT_NAME}_SRCS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_default ${${PROJECT_NAME}_INCLUDES})

IF(BUILD_SDK_INSTALLER)
    ARC_INSTALL_LIBRARIES_SRC(${PROJECT_NAME})
ENDIF()
