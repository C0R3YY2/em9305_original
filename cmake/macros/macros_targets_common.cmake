# Process the list of libraries in case there are some wrappers used.
# INTERNAL MACRO, DO NOT USE IT DIRECTLY!
MACRO(__EXPAND_LIBS_LIST out hw_di)
    SET(${out})
    FOREACH(lib ${ARGN})
        STRING(FIND ${lib} "__" FIND_RES_IDX)
        IF(NOT ${FIND_RES_IDX} EQUAL 0)
            SET(${out} ${${out}} ${lib})
        ELSE()
            SET(libs)
            IF(DEFINED ${lib}${hw_di})
                __EXPAND_LIBS_LIST(libs "${hw_di}" ${${lib}${hw_di}})
            ELSEIF(DEFINED ${lib})
                __EXPAND_LIBS_LIST(libs "${hw_di}" ${${lib}})
            ELSE()
                MESSAGE(SEND_ERROR "Invalid wrapper of libraries: '${lib}'")
            ENDIF()
            SET(libs_list ${libs_list} ${libs})
        ENDIF()
    ENDFOREACH()
ENDMACRO(__EXPAND_LIBS_LIST)


# Link some libraries to a target in all the build configs.
MACRO(ARC_LINK_LIBRARIES target)
    # Link the libraries in all the build configs.
    FOREACH(build ${BUILD_CONFIGS})
        # Get the build target parameters.
        ARC_GET_BUILD_TARGET(${build} ${target})
        # Be sure the target exists.
        IF(TARGET ${build_target})
            # Get the target type.
            GET_TARGET_PROPERTY(target_type ${build_target} TYPE)
            # this enables the creation of utility target without "_di" for default target.
            IF(NOT ("${target_type}" STREQUAL "UTILITY"))
                # Process the list of libraries in case there are some wrappers used.
                __EXPAND_LIBS_LIST(libs_list "" ${ARGN})

                # Link all libraries passed in parameters.
                FOREACH(lib ${libs_list})
                    SET(library_type "")

                    # Check if the library is a logical target (not a pre-compiled library).
                    IF(NOT EXISTS ${lib})
                        # Check if the target is a library.
                        IF(NOT "${target_type}" STREQUAL "EXECUTABLE")
                            # Be sure that if we want to link with a library that can be HW Design Iteration specific, the same
                            # HW Design Iteration specific versions of this library also exist.
                            FOREACH(hw_di ${HW_DI_LIST})
                                IF(TARGET ${lib}${build_postfix}_${hw_di} AND NOT TARGET ${build_target}_${hw_di})
                                    MESSAGE(FATAL_ERROR "The library '${lib}' contains some HW Design Iterations specific versions that the target '${target}' doesn't!")
                                ENDIF()
                            ENDFOREACH()
                        ENDIF()
                        # Be sure the library exists.
                        IF(TARGET ${lib}${build_postfix})
                            SET(lib "${lib}${build_postfix}")
                        ELSE()
                            MESSAGE(FATAL_ERROR "Library '${lib}' not found; Cannot be linked with '${target}'!")
                        ENDIF()
                        # Get the library type.
                        GET_TARGET_PROPERTY(library_type ${lib} TYPE)
                    ENDIF()

                    IF("${library_type}" STREQUAL "OBJECT_LIBRARY")
                        # Add the objects from the object library.
                        TARGET_SOURCES(${build_target} PRIVATE $<TARGET_OBJECTS:${lib}>)
                    ELSE()
                        # Link the library.
                        TARGET_LINK_LIBRARIES(${build_target} PRIVATE ${lib})
                    ENDIF()
                ENDFOREACH()
            ENDIF()
        ENDIF()
        # Try as well using the HW Design Iteration postfix.
        FOREACH(hw_di ${HW_DI_LIST})
            # Be sure the target exists.

            IF(TARGET ${build_target}_${hw_di})
                # Process the list of libraries in case there are some wrappers used.
                __EXPAND_LIBS_LIST(libs_list "${hw_di}" ${ARGN})
                # Get the target type.
                # this enables the creation of utility target without "_di" for default target.
                GET_TARGET_PROPERTY(target_type ${build_target}_${hw_di} TYPE)
                IF (NOT "${target_type}" STREQUAL "UTILITY")
                    # Link all libraries passed in parameters.
                    FOREACH(lib ${libs_list})
                        SET(library_type "")
                        # Check if the library is a logical target (not a pre-compiled library).
                        IF(NOT EXISTS ${lib})
                            # Check if a HW Design Iteration specific library exists for this HW Design Iteration.
                            IF(TARGET ${lib}${build_postfix}_${hw_di})
                                SET(lib "${lib}${build_postfix}_${hw_di}")
                            # If not, check if the standard library exists.
                            ELSEIF(TARGET ${lib}${build_postfix})
                                SET(lib "${lib}${build_postfix}")
                            ELSE()
                                MESSAGE(FATAL_ERROR "Library '${lib}' not found; Cannot be linked with '${build_target}_${hw_di}'!")
                            ENDIF()
                            # Get the library type.
                            GET_TARGET_PROPERTY(library_type ${lib} TYPE)
                        ENDIF()
                        STRING(FIND ${build_target} "emcore_" IS_EMCORE_TARGET)
                        STRING(FIND ${build_target} "custom_emcore_" IS_CUSTOM_EMCORE_TARGET)

                        IF("${library_type}" STREQUAL "OBJECT_LIBRARY")
                            # Add the objects from the object library.
                            TARGET_SOURCES(${build_target}_${hw_di} PRIVATE $<TARGET_OBJECTS:${lib}>)
                            IF(IS_EMCORE_TARGET EQUAL 0 OR IS_CUSTOM_EMCORE_TARGET EQUAL 0)
                                TARGET_SOURCES(${build_target}_no_crc_check_${hw_di} PRIVATE $<TARGET_OBJECTS:${lib}>)
                            ENDIF()
                        ELSE()
                            # Link the library.
                            TARGET_LINK_LIBRARIES(${build_target}_${hw_di} PRIVATE ${lib})
                            IF(IS_EMCORE_TARGET EQUAL 0 OR IS_CUSTOM_EMCORE_TARGET EQUAL 0)
                                TARGET_LINK_LIBRARIES(${build_target}_no_crc_check_${hw_di} PRIVATE ${lib})
                            ENDIF()
                        ENDIF()
                    ENDFOREACH()
                ENDIF()
            ENDIF()
        ENDFOREACH()
    ENDFOREACH()
ENDMACRO(ARC_LINK_LIBRARIES)

# Include some directories in a target for all the build configs.
MACRO(ARC_INCLUDE_DIRECTORIES target)
    # Include the directories in all the build configs.
    FOREACH(build ${BUILD_CONFIGS})
        # Get the build target parameters.
        ARC_GET_BUILD_TARGET(${build} ${target})
        # Be sure the target exists.
        # STRING(FIND ${build_target} "_di" FIND_RES_IDX) # add libraries to di specific targets
        # this enables the creation of utility target without "_di" for default target.

        IF(TARGET ${build_target})
            GET_TARGET_PROPERTY(target_type ${build_target} TYPE)
            ## We must be able to know whether the target is a lib or an application. A lib may be generated without _di extension.
            # MESSAGE("ARC_INCLUDE_DIRECTORIES - target_type for ${build_target}: ${target_type}")
            GET_TARGET_PROPERTY(target_type ${build_target} TYPE)
            # IF( (target_type STREQUAL "STATIC_LIBRARY" OR target_type STREQUAL "OBJECT_LIBRARY" OR NOT (target_type STREQUAL "UTILITY")))
            IF(NOT (target_type STREQUAL "UTILITY"))
                # Include the directories in the target.
                TARGET_INCLUDE_DIRECTORIES(${build_target} PRIVATE ${ARGN})

                # Include the EMCORE headers (only for applications that use EMCORE).
                GET_TARGET_PROPERTY(app_emcore_flavor ${build_target} EMCORE_FLAVOR)
                GET_TARGET_PROPERTY(app_emcore_version ${build_target} EMCORE_GIT_VERSION_STR)
                IF(NOT "${app_emcore_version}" STREQUAL "app_emcore_version-NOTFOUND" AND NOT "${app_emcore_flavor}" STREQUAL "app_emcore_flavor-NOTFOUND")
                    TARGET_INCLUDE_DIRECTORIES(${build_target} PRIVATE "${EMCORE_BIN_DIR}/${app_emcore_version}/${app_emcore_flavor}/includes")
                ENDIF()

                # Include the ROM headers.
                # Include all ROM headers from the most recent one as it is a generic target.
                SET(HW_DI_LIST_REVERSED ${HW_DI_LIST})
                LIST(REVERSE HW_DI_LIST_REVERSED)
                FOREACH(hw_di ${HW_DI_LIST_REVERSED})
                    TARGET_INCLUDE_DIRECTORIES(${build_target} PRIVATE "${ROM_DIR}/bin/${ROM_VERSION_${hw_di}}/includes")
                ENDFOREACH()
            ENDIF()
        ENDIF()

        # Try as well using the HW Design Iteration postfix.
        FOREACH(hw_di ${HW_DI_LIST})
            # Be sure the target exists.
            IF(TARGET ${build_target}_${hw_di})
                # Include the directories in the target.
                TARGET_INCLUDE_DIRECTORIES(${build_target}_${hw_di} PRIVATE ${ARGN})
                STRING(FIND ${build_target} "emcore_" IS_EMCORE_TARGET)
                STRING(FIND ${build_target} "custom_emcore_" IS_CUSTOM_EMCORE_TARGET)

                IF(IS_EMCORE_TARGET EQUAL 0 OR IS_CUSTOM_EMCORE_TARGET EQUAL 0)
                    TARGET_INCLUDE_DIRECTORIES(${build_target}_no_crc_check_${hw_di} PRIVATE ${ARGN})
                ENDIF()
                # Include the EMCORE headers (only for applications that use EMCORE).
                GET_TARGET_PROPERTY(app_emcore_flavor ${build_target}_${hw_di} EMCORE_FLAVOR)
                GET_TARGET_PROPERTY(app_emcore_version ${build_target}_${hw_di} EMCORE_GIT_VERSION_STR)
                IF(NOT "${app_emcore_version}" STREQUAL "app_emcore_version-NOTFOUND" AND NOT "${app_emcore_flavor}" STREQUAL "app_emcore_flavor-NOTFOUND")
                    TARGET_INCLUDE_DIRECTORIES(${build_target}_${hw_di} PRIVATE "${EMCORE_BIN_DIR}/${app_emcore_version}/${app_emcore_version}/includes")
                ENDIF()

                # Include the ROM headers.
                TARGET_INCLUDE_DIRECTORIES(${build_target}_${hw_di} PRIVATE "${ROM_DIR}/bin/${ROM_VERSION_${hw_di}}/includes")
                IF(IS_EMCORE_TARGET EQUAL 0)
                    TARGET_INCLUDE_DIRECTORIES(${build_target}_no_crc_check_${hw_di} PRIVATE "${ROM_DIR}/bin/${ROM_VERSION_${hw_di}}/includes")
                ENDIF()
            ENDIF()
        ENDFOREACH()
    ENDFOREACH()
ENDMACRO(ARC_INCLUDE_DIRECTORIES)
