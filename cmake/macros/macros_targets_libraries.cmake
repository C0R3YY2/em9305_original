# Called to clear the last library location.
MACRO(LIB_CLEAR_LOCATION)
    SET(LIB_IS_TEST         False)
    SET(LIB_IS_IN_ROM       False)
    SET(LIB_IS_IN_NVM       False)
    SET(LIB_IS_IN_IRAM      False)
    SET(LIB_CFLAGS          "")
ENDMACRO(LIB_CLEAR_LOCATION)

# Build TEST library.
MACRO(LIB_TEST)
    LIB_CLEAR_LOCATION()
    SET(LIB_IS_TEST True)
    SET(LIB_CFLAGS "-DTEST_CODE")
ENDMACRO(LIB_TEST)

# Build library in ROM.
MACRO(LIB_IN_ROM)
    LIB_CLEAR_LOCATION()
    SET(LIB_IS_IN_ROM True)
    SET(LIB_CFLAGS "-DROM_CODE")
ENDMACRO(LIB_IN_ROM)

# Build library in NVM.
MACRO(LIB_IN_NVM)
    LIB_CLEAR_LOCATION()
    SET(LIB_IS_IN_NVM True)
    SET(LIB_CFLAGS "-DNVM_CODE")
ENDMACRO(LIB_IN_NVM)

# Build library in IRAM.
MACRO(LIB_IN_IRAM)
    LIB_CLEAR_LOCATION()
    SET(LIB_IS_IN_IRAM True)
    SET(LIB_CFLAGS "-DIRAM_CODE")
ENDMACRO(LIB_IN_IRAM)

MACRO(ADD_SUBDIRECTORY_LIB subdirectory mode)
    SET(include_enable False)
    IF(${mode} STREQUAL "ROM")
        IF(DEFINED ROM_BUILD)
            ADD_SUBDIRECTORY(${subdirectory})
            SET(include_enable True)
        ENDIF()
    ELSEIF(${mode} STREQUAL "CONF")
        IF(NOT DEFINED ROM_BUILD)
            ADD_SUBDIRECTORY(${subdirectory})
        ENDIF()
    ELSEIF(${mode} STREQUAL "EMCORE")
        IF(EMCORE_DEV STREQUAL "true")
            IF(NOT DEFINED ROM_BUILD)
                ADD_SUBDIRECTORY(${subdirectory})
            ENDIF()
        ENDIF()
    ELSE()
        IF(DEFINED ROM_BUILD)
            SET(include_enable True)
        ENDIF()
        ADD_SUBDIRECTORY(${subdirectory})
    ENDIF()
    IF(${include_enable})
        FILE(GLOB_RECURSE headers "${subdirectory}/includes/*.h")
        FOREACH(header ${headers})
            IF(NOT (${header} IN_LIST header_black_list))
                MESSAGE("Install header: ${header}")
                INSTALL(FILES "${header}" DESTINATION "includes")
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO(ADD_SUBDIRECTORY_LIB)

# Add a new library for all the build configs.
# INTERNAL MACRO, DO NOT USE IT DIRECTLY!
MACRO(__ARC_ADD_LIBRARY target type lib_hw_di_list lib_srcs)
    # Add the library in all the build configs.
    FOREACH(build ${BUILD_CONFIGS})
        # Get the build target parameters.
        ARC_GET_BUILD_TARGET(${build} ${target})

        # Add the library for the selected HW Design Iterations.
        # It also add a non-specific version of the library.
        FOREACH(hw_di IN ITEMS "" ${lib_hw_di_list})
            SET(IGNORE_THIS_DI False)
            # Add a postfix and a compilation flag if this is a HW DI specific libarary.
            IF("${hw_di}" STREQUAL "")
                SET(build_hw_di_postfix "")
                SET(build_hw_di_flag    "")
            ELSE()
                LIST(FIND HW_DI_INDEX "${hw_di}" hw_di_idx)
                IF(${hw_di_idx} EQUAL -1)
                    IF(${LDRA_BUILD})
                        MESSAGE("LDRA_BUILD just setting dev Design Iteration not included '${hw_di}'!")
                    ELSE()
                        # MESSAGE(FATAL_ERROR "Invalid HW Design Iteration '${hw_di}'!")
                        # MESSAGE("HW Design Iteration '${hw_di}' Invalid or not supported anymore, skipping!")
                        SET(IGNORE_THIS_DI True)
                    ENDIF()
                ENDIF()
                SET(build_hw_di_postfix "_${hw_di}")
                SET(build_hw_di_flag    "-DHW_DI=${hw_di_idx}")
            ENDIF()
            IF (NOT IGNORE_THIS_DI)
                # Get the source files from the arguments.
                IF(DEFINED ${lib_srcs}${build_hw_di_postfix})
                    SET(TARGET_SRCS ${${lib_srcs}${build_hw_di_postfix}})
                ELSE()
                    SET(TARGET_SRCS ${${lib_srcs}})
                ENDIF()

                # Add the library.
                IF(NOT LIB_IS_TEST AND build_all_target)
                    ADD_LIBRARY(${build_target}${build_hw_di_postfix} ${type} ${TARGET_SRCS})
                ELSE()
                    ADD_LIBRARY(${build_target}${build_hw_di_postfix} ${type} EXCLUDE_FROM_ALL ${TARGET_SRCS})
                ENDIF()

                # Use the Source Clean tool to clean the source code (except in SDK).
                IF(NOT SDK_BUILD)
                    ADD_DEPENDENCIES(${build_target}${build_hw_di_postfix} source_clean)
                ENDIF()

                # Add the common compilation flags according to the build config and library location.
                ADD_TARGET_COMPILE_FLAGS(${build_target}${build_hw_di_postfix} "${build_cflags} ${build_hw_di_flag} ${LIB_CFLAGS}")

                # Add the jli.h header as a dependency for all source files.
                GET_TARGET_PROPERTY(all_sources "${build_target}${build_hw_di_postfix}" SOURCES)
                SET_SOURCE_FILES_PROPERTIES(${all_sources} PROPERTIES
                    OBJECT_DEPENDS "${COMMON_DIR}/${TARGET_HW}/includes/jli.h"
                )

                # Generate the assembly dump if required.
                IF(GENERATE_ASSEMBLY_DUMP AND NOT "${type}" STREQUAL "OBJECT")
                    IF(NOT SDK_BUILD AND INLINE_CODE_IN_ASSEMBLY_DUMP)
                        SET(ELFDUMP_SCRIPT ${CMAKE_DIR}/scripts/elfdump_intermixed.cmake)
                    ELSE()
                        SET(ELFDUMP_SCRIPT ${CMAKE_DIR}/scripts/elfdump.cmake)
                    ENDIF()
                    ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} ARGS
                            -DIN=${CMAKE_STATIC_LIBRARY_PREFIX}${build_target}${build_hw_di_postfix}.a
                            -DOUT=${CMAKE_STATIC_LIBRARY_PREFIX}${build_target}${build_hw_di_postfix}.asm
                            -P ${ELFDUMP_SCRIPT}
                        COMMENT "Generating ${CMAKE_STATIC_LIBRARY_PREFIX}${build_target}${build_hw_di_postfix}.asm" VERBATIM
                    )
                ENDIF()
            ENDIF()
        ENDFOREACH()
    ENDFOREACH()

    # Be sure the defined location can be used only once.
    LIB_CLEAR_LOCATION()
ENDMACRO(__ARC_ADD_LIBRARY)

# Add a new STANDARD library for all the build configs.
# The library location can be defined with one 'LIB_IN_*' macro.
MACRO(ARC_ADD_LIBRARY target)
    SET(lib_srcs ${ARGN})
    __ARC_ADD_LIBRARY(${target} "" "" lib_srcs)
ENDMACRO(ARC_ADD_LIBRARY)

# Add a new HW Design Iteration specific STANDARD library for all the build configs.
# The library location can be defined with one 'LIB_IN_*' macro.
# Note: As the source files can be different for each HW Design Iteration, you
#   cannot directly pass the list of source files in parameters. Instead you
#   have to create a generic source files list and other optional lists for
#   each HW Design Iteration that requires a specific list of source files.
#   Then, pass only the name of the generic list in parameter. If no list is
#   created for a HW Design Iteration, the generic one will be used instead.
# Example:
#   SET(LIB_XY_SRCS      aaa.c bbb.c)   # Generic library (mandatory)
#   SET(LIB_XY_SRCS_di01 aaa.c ccc.c)   # di01 specific library
#   ARC_ADD_HW_DI_SPECIFIC_LIBRARY(XY "di01;di02" LIB_XY_SRCS)
MACRO(ARC_ADD_HW_DI_SPECIFIC_LIBRARY target lib_hw_di_list lib_srcs)
    __ARC_ADD_LIBRARY(${target} "" "${lib_hw_di_list}" ${lib_srcs})
ENDMACRO(ARC_ADD_HW_DI_SPECIFIC_LIBRARY)

# Add a new OBJECT library for all the build configs.
# The library location can be defined with one 'LIB_IN_*' macro.
MACRO(ARC_ADD_OBJECT_LIBRARY target)
    SET(lib_srcs ${ARGN})
    __ARC_ADD_LIBRARY(${target} "OBJECT" "" lib_srcs)
ENDMACRO(ARC_ADD_OBJECT_LIBRARY)

# Add a new HW Design Iteration specific OBJECT library for all the build configs.
# The library location can be defined with one 'LIB_IN_*' macro.
# Note: As the source files can be different for each HW Design Iteration, you
#   cannot directly pass the list of source files in parameters. Instead you
#   have to create a generic source files list and other optional lists for
#   each HW Design Iteration that requires a specific list of source files.
#   Then, pass only the name of the generic list in parameter. If no list is
#   created for a HW Design Iteration, the generic one will be used instead.
# Example:
#   SET(LIB_XY_SRCS      aaa.c bbb.c)   # Generic library (mandatory)
#   SET(LIB_XY_SRCS_di01 aaa.c ccc.c)   # di01 specific library
#   ARC_ADD_HW_DI_SPECIFIC_OBJECT_LIBRARY(XY "di01;di02" LIB_XY_SRCS)
MACRO(ARC_ADD_HW_DI_SPECIFIC_OBJECT_LIBRARY target lib_hw_di_list lib_srcs)
    __ARC_ADD_LIBRARY(${target} "OBJECT" "${lib_hw_di_list}" ${lib_srcs})
ENDMACRO(ARC_ADD_HW_DI_SPECIFIC_OBJECT_LIBRARY)

# Set a property for all specified hardware revisions.
# Example:
#    ADD_HW_DI_SPECIFIC_PROPERTY(
#       my_target
#       "di01;di02" INTERFACE_LINK_OPTIONS "-u" "programHeader")
FUNCTION(ADD_HW_DI_SPECIFIC_PROPERTY target revisions property)
    LIST(TRANSFORM revisions PREPEND "${target}_")
    SET_PROPERTY(
        TARGET ${revisions}
        APPEND
        PROPERTY "${property}" ${ARGN}
    )
ENDFUNCTION()

# Function for prepending a header to a target

# \brief: link a firmware header object to any target
# \param: target -> name of the target header should be linked to
# \param: segment_type -> type of firmware payload: either 'bootloader',
# 'emcore' or 'application' for now.
# \param: emcore_flavor -> one of EMCORE_FLAVORS (see top level CMakeLists)
#
# Example:
# ADD_HEADER_TO_TARGET(my_target "application" "standard" "0x0101")
FUNCTION(ADD_HEADER_TO_TARGET target segment_type emcore_flavor firmware_version)
    SET(_segment_types
        "emcore"      # section_code = 0x01
        "application" # section_code = 0x03
        "bootloader"  # section_code = 0x04
        "monolithic"  # section_code = TBD, for now it is same as 'application'
        "firmware_updater" # section_code = 0x00
    )

    SET(fw_header_path "${LIBS_DIR}/firmware_header")
    LIST(FIND _segment_types "${segment_type}" segment_type_index)

    GET_EMCORE_FLAVOR_ID(${emcore_flavor} emcore_flavor_id is_custom_emcore)
    # Valid emcore flavor is required for emcore or application builds.
    IF (${emcore_flavor_id} EQUAL -1 AND (${segment_type_index} EQUAL 0 OR ${segment_type_index} EQUAL 1))
        MESSAGE(FATAL_ERROR "${emcore_flavor} is an invalid EMCORE_FLAVOR")
    ENDIF()
    SET(fw_header_compile_options
        "-DHEADER_ENABLED"
        "-DFIRMWARE_VERSION=${firmware_version}"
        "-DNO_FLINT_ERRORS"
    )
    IF (${segment_type_index} EQUAL 0) # EMCORE
        SET(fw_header_compile_options
            ${fw_header_compile_options}
            "-DEMCORE_FLAVOR_ID=${emcore_flavor_id}"
            "-DEMCORE_BUILD"
        )
    ELSEIF(${segment_type_index} EQUAL 1) # APPLICATION
        SET(fw_header_compile_options
            ${fw_header_compile_options}
            "-DEMCORE_FLAVOR_ID=${emcore_flavor_id}"
            "-DUSERAPP_BUILD"
        )
    ELSEIF(${segment_type_index} EQUAL 2) # BOOTLOADER
        SET(fw_header_compile_options
            ${fw_header_compile_options}
            "-DEMCORE_FLAVOR_ID=${emcore_flavor_id}"
            "-DBOOTLOADER_BUILD"
        )
    ELSEIF(${segment_type_index} EQUAL 3) # MONOLITHIC
        SET(fw_header_compile_options
            ${fw_header_compile_options}
            "-DEMCORE_FLAVOR_ID=0"
            "-DMONOLITHIC_BUILD"
        )
    ELSEIF (${segment_type_index} EQUAL 4) # Firmware Updater
        SET(fw_header_compile_options
            ${fw_header_compile_options}
            "-DEMCORE_FLAVOR_ID=0"
            "-DFW_UPDATER_BUILD"
        )
    ELSE()
        MESSAGE(FATAL_ERROR "${segment_type} is an invalid segment type (ID ${_index}).")
    ENDIF()

    IF (${IMAGE_CRC_CHECK})
        SET(fw_header_compile_options
            ${fw_header_compile_options}
            "-DIMAGE_CRC_CHECK"
        )
    ENDIF()

    ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${target}
        ${fw_header_compile_options}
    )
ENDFUNCTION()
