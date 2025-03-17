# Called to clear the last application location.
# Optional argument: ROM postfix.
MACRO(APP_CLEAR_LOCATION)
    SET(APP_IS_TEST             False)
    SET(APP_IS_IN_ROM           False)
    SET(APP_IS_IN_NVM           False)
    SET(APP_IS_IN_IRAM          False)
    SET(APP_IS_USING_EMCORE     False)
    SET(APP_IS_BOOTLOADER       False)
    SET(APP_IS_EMCORE           False)
    SET(CUSTOM_EMCORE           False)
    SET(APP_IS_NVMBOOTLOADER    False)
    SET(APP_IS_STANDALONE       False)
    SET(APP_IS_FIRMWARE_UPDATER False)
    SET(APP_LINKER_FOLDER       "")
    SET(APP_CFLAGS              "")
    SET(APP_FULL_ROM_SYM        False)
    SET(APP_EMCORE_FLAVOR       "")
    SET(APP_EMCORE_VERSION      "")
    SET(APP_SEMANTIC_VERSION    "0x0000") # default version of the app
    SET(INCLUDE_NVMBL_IN_IHEX   True) # default version of the app

    IF(${ARGC} EQUAL 1)
        SET(ROM_BASENAME    ${ROM_BASENAME_ASIC}${ARGV0})
    ELSE()
        SET(ROM_BASENAME    ${ROM_BASENAME_ASIC}${ROM_POSTFIX_SPI})
    ENDIF()
ENDMACRO(APP_CLEAR_LOCATION)

# Set the version number for some application
# Argument: semantic version as hex string. For example "0x1010"
MACRO(APP_VERSION sem_version)
    SET(APP_SEMANTIC_VERSION ${sem_version})
ENDMACRO(APP_VERSION)

# Exclude the nvm_bootloader from the ihex file
MACRO(EXCLUDE_NVMBL_FROM_IHEX)
    SET(INCLUDE_NVMBL_IN_IHEX False)
ENDMACRO(EXCLUDE_NVMBL_FROM_IHEX)

# Build TEST application.
MACRO(APP_TEST)
    APP_CLEAR_LOCATION()
    SET(APP_IS_TEST True)
    SET(APP_LINKER_FOLDER "TEST")
    IF(MW_VERSION STREQUAL "2019.09")
        SET(APP_CFLAGS "-DTEST_CODE -Hpurge")
    ELSE()
        SET(APP_CFLAGS "-DTEST_CODE -Wno-error=unused-but-set-variable -Wno-error=pointer-to-int-cast -Hpurge")
    ENDIF()
    SET(APP_LDFLAGS "-Hentry=gVectors -Ball_archive -Hnolib -Hpurge")
ENDMACRO(APP_TEST)

# Build application in ROM.
MACRO(APP_IN_ROM)
    APP_CLEAR_LOCATION()
    SET(APP_IS_IN_ROM True)
    SET(APP_LINKER_FOLDER "ROM")
    SET(APP_CFLAGS "-DROM_CODE")
    SET(APP_LDFLAGS "-Hentry=gVectors -Ball_archive -Hnolib")
ENDMACRO(APP_IN_ROM)

# Helper for setting C flags and Linker flags.
# Optional argument: entry symbol
MACRO(_APP_STD_FLAGS)
    SET(entry_symbol "NVM_Entry")
    IF(${ARGC} GREATER 0)
        SET(entry_symbol ${ARGV0})
    ENDIF()
    SET(APP_CFLAGS "-DNVM_CODE -Hpurge")
    IF(MW_FOR_EM)
        SET(APP_LDFLAGS "-Hentry=${entry_symbol} -Bno_all_archive -Hpurge")
    ELSE()
        SET(APP_LDFLAGS "-Hentry=${entry_symbol} -Bno_all_archive -Hpurge -Hlib=${LIBS_DIR}/third_party/Metaware/bin/${MW_VERSION}")
    ENDIF()
ENDMACRO()

# Build application in NVM.
# Optional argument: ROM postfix.
MACRO(APP_IN_NVM)
    APP_CLEAR_LOCATION(${ARGN})
    SET(APP_IS_IN_NVM True)
    SET(APP_LINKER_FOLDER "NVM")
    _APP_STD_FLAGS()
ENDMACRO(APP_IN_NVM)

# Build emcore in NVM
# Argument: EMCORE version/flavor.
MACRO(EMCORE_IN_NVM emcore_flavor)
    APP_CLEAR_LOCATION(${ARGN})
    # tricks to ensure emcore-standard keeps its ID
    SET(emcore_idx "")
    SET(is_custom_emcore False)
    GET_EMCORE_FLAVOR_ID(${emcore_flavor} emcore_idx is_custom_emcore)
    IF(${emcore_idx} EQUAL -1)
        MESSAGE(FATAL_ERROR "Invalid EMCORE flavor '${emcore_flavor}'!")
    ENDIF()
    SET(APP_IS_IN_NVM True)
    SET(CUSTOM_EMCORE False)
    SET(APP_LINKER_FOLDER "NVM")
    _APP_STD_FLAGS()
    SET(APP_IS_EMCORE True)
    EXCLUDE_NVMBL_FROM_IHEX()
    SET(APP_EMCORE_FLAVOR "${emcore_flavor}")
    SET(APP_EMCORE_VERSION "${EMCORE_VERSION}")
ENDMACRO(EMCORE_IN_NVM)

# Build custom emcore in NVM
# Argument: Custom EMCORE version/flavor.
MACRO(CUSTOM_EMCORE_IN_NVM custom_emcore_flavor)
    APP_CLEAR_LOCATION(${ARGN})
    # tricks to ensure emcore keeps its ID
    GET_EMCORE_FLAVOR_ID(${custom_emcore_flavor} custom_emcore_idx is_custom_emcore)
    IF(${custom_emcore_idx} EQUAL -1)
        MESSAGE(FATAL_ERROR "Invalid EMCORE flavor '${custom_emcore_flavor}'!")
    ENDIF()
    SET(APP_IS_IN_NVM True)
    SET(CUSTOM_EMCORE True)
    SET(APP_LINKER_FOLDER "NVM")
    _APP_STD_FLAGS()
    SET(APP_IS_EMCORE True)
    EXCLUDE_NVMBL_FROM_IHEX()
    SET(APP_EMCORE_FLAVOR "${custom_emcore_flavor}")
    SET(APP_EMCORE_VERSION "${CUSTOM_EMCORE_VERSION}")
    MESSAGE("Custom EMCore FLavor: ${APP_EMCORE_FLAVOR}")
ENDMACRO(CUSTOM_EMCORE_IN_NVM)


# Build application in IRAM.
# Optional argument: ROM postfix.
MACRO(APP_IN_IRAM)
    APP_CLEAR_LOCATION(${ARGN})
    SET(APP_IS_IN_IRAM True)
    SET(APP_LINKER_FOLDER "IRAM")
    SET(APP_CFLAGS "-DIRAM_CODE")
    SET(APP_LDFLAGS "-Hentry=IRAM_main -Ball_archive -Hnolib")
ENDMACRO(APP_IN_IRAM)

# Build application using EMCORE in NVM.
# Argument: EMCORE variant.
# Optional argument: EMCORE version
MACRO(APP_USING_EMCORE emcore_flavor)
    APP_CLEAR_LOCATION()
    GET_EMCORE_FLAVOR_ID(${emcore_flavor} emcore_idx is_custom_emcore)
    IF(${emcore_idx} EQUAL -1)
        MESSAGE(FATAL_ERROR "Invalid EMCORE flavor '${emcore_flavor}'!")
    ENDIF()

    SET(APP_IS_USING_EMCORE True)
    SET(APP_IS_USING_CUSTOM_EMCORE ${is_custom_emcore})

    EXCLUDE_NVMBL_FROM_IHEX()
    SET(APP_LINKER_FOLDER "APP")
    _APP_STD_FLAGS("APP_EntryFunction")
    SET(APP_EMCORE_FLAVOR "${emcore_flavor}")
    # Check if optional parameter (EMCore version) is present
    # In this case, we use it for this specific target.
    IF(${ARGC} EQUAL 2)
        SET(APP_EMCORE_VERSION ${ARGV1})
    ELSEIF(DEFINED EMCORE_VERSION)
        SET(APP_EMCORE_VERSION "${EMCORE_VERSION}")
    ELSEIF(SDK_BUILD)
        MESSAGE("No specified version, using latest version available in SDK")
        SET(APP_EMCORE_VERSION "v4.1.0")
    ELSE()
        GIT_DESCRIBE("sdk-emb" APP_EMCORE_VERSION ${ROOT_DIR})
    ENDIF()
    # MESSAGE("Genrating application using EMCORE ${APP_EMCORE_VERSION} FLAVOR: ${emcore_flavor}")
ENDMACRO(APP_USING_EMCORE)

# Build nvm bootloader in NVM.
# Optional argument: ROM postfix.
MACRO(NVMBOOTLOADER_IN_NVM)
    APP_CLEAR_LOCATION(${ARGN})
    SET(APP_IS_IN_NVM True)
    SET(APP_LINKER_FOLDER "NVM_BOOTLOADER")
    SET(APP_IS_NVMBOOTLOADER True)
    EXCLUDE_NVMBL_FROM_IHEX()
    SET(APP_CFLAGS "-DNVM_CODE -Hpurge")
    SET(APP_LDFLAGS "${APP_LDFLAGS} -Hentry=nvm_bootloader -Hpurge -Bdefine:APP_IS_NVMBOOTLOADER=1")
ENDMACRO(NVMBOOTLOADER_IN_NVM)

# Build bootloader in NVM.
# Optional argument: ROM postfix.
MACRO(BOOTLOADER_IN_NVM)
    APP_IN_NVM(${ARGN})
    SET(APP_IS_BOOTLOADER True)
    EXCLUDE_NVMBL_FROM_IHEX()
    SET(APP_LDFLAGS "${APP_LDFLAGS} -Bdefine:APP_IS_BOOTLOADER=1")
ENDMACRO(BOOTLOADER_IN_NVM)

# Build Firmware Updater in NVM.
# Optional argument: ROM postfix.
MACRO(FIRMWARE_UPDATER_IN_NVM)
    APP_IN_NVM(${ARGN})
    SET(APP_LINKER_FOLDER "FW_UPDATER")
    SET(APP_IS_FIRMWARE_UPDATER True)
    EXCLUDE_NVMBL_FROM_IHEX()
ENDMACRO(FIRMWARE_UPDATER_IN_NVM)


# Build a completely standalone app in NVM. Will not get any nvm_bootloader or
# program headers.
MACRO(STANDALONE_IN_NVM)
    APP_CLEAR_LOCATION()
    SET(APP_IS_STANDALONE True)
    EXCLUDE_NVMBL_FROM_IHEX()
    SET(APP_LINKER_FOLDER "HW_FUNC_TEST")
    _APP_STD_FLAGS()
ENDMACRO(STANDALONE_IN_NVM)


# Use full ROM symbol list. Must be called after APP_IN_*().
MACRO(FULL_ROM_SYM)
    SET(APP_FULL_ROM_SYM True)
ENDMACRO(FULL_ROM_SYM)


# Add a new executable for all the build configs.
# The application location shall be defined with one 'APP_IN_*' macro.
MACRO(ARC_EXECUTABLE target)
    # Be sure the app location has been defined.
    IF(NOT (APP_IS_TEST OR APP_IS_IN_ROM OR APP_IS_IN_NVM OR APP_IS_IN_IRAM
            OR APP_IS_USING_EMCORE OR APP_IS_STANDALONE)
    )
        MESSAGE(FATAL_ERROR "Location not defined for the target '${target}'!")
    ENDIF()

    # Check if the project name is equal to the project directory.
    # EMCore is an exception as the emcore project is located in emcore/dev.
    GET_FILENAME_COMPONENT(PROJECT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    IF(NOT ${PROJECT_NAME} STREQUAL ${PROJECT_DIRECTORY} AND NOT ${PROJECT_NAME} STREQUAL "emcore" AND NOT ${PROJECT_NAME} STREQUAL "custom_emcore")
        MESSAGE(WARNING "Project name '${PROJECT_NAME}' and directory '${PROJECT_DIRECTORY}' mismatch!")
    ENDIF()

    # Keep only the HW Design Iterations 'dev' for ROM & TESTS applications.
    # Keep only HW design Iteration 'dev' also for targets aimed for LDRA testing.
    STRING(FIND ${target} "_ldra" idx)
    IF(APP_IS_IN_ROM OR APP_IS_TEST)
        LIST(GET HW_DI_LIST 0 app_hw_di_list)
        SET(build_config_list "${BUILD_CONFIGS}")
    ELSEIF(${idx} GREATER_EQUAL 0) # LDRA Config
        LIST(GET HW_DI_LIST 0 app_hw_di_list)
        SET(build_config_list "__fpga")
    ELSE()
        SET(app_hw_di_list "${HW_DI_LIST}")
        SET(build_config_list "${BUILD_CONFIGS}")
    ENDIF()

    # Get the source files from the arguments.
    SET(TARGET_SRCS ${ARGN})

    # Always add the JLI in ROM and TEST apps.
    IF(APP_IS_IN_ROM OR APP_IS_TEST)
        SET(TARGET_SRCS ${TARGET_SRCS}
            "${COMMON_DIR}/${TARGET_HW}/ROM/jlitab.s"
        )
    ENDIF()

    # Just to display what's being used for which target.
    # Keep the comment for debug purpose
    # IF(APP_IS_USING_EMCORE)
    #     MESSAGE("Target ${target} based on EMCore ${APP_EMCORE_FLAVOR} version ${APP_EMCORE_VERSION}")
    # ENDIF()
    # Add the application in all the build configs.

    FOREACH(build ${build_config_list})
        # Get the build target parameters.
        ARC_GET_BUILD_TARGET(${build} ${target})

        # Do not add executable for TEST apps in some build configs.
        IF(NOT APP_IS_TEST OR NOT build_omit_tests)
            # Select the appropriate ROM target.
            IF(APP_IS_IN_ROM)
                # For ROM applications, keep the build target.
                SET(ROM_TARGET ${build_target})
            ELSEIF(${build_name} STREQUAL "ASIC")
                # Real ASIC exists only in SPI!
                SET(ROM_TARGET ${ROM_BASENAME_ASIC}${ROM_POSTFIX_SPI}${build_postfix})
            ELSE()
                # Select the ROM defined by the user with the 'APP_IN_*' macro.
                SET(ROM_TARGET ${ROM_BASENAME}${build_postfix})
            ENDIF()

            # Add the application for all HW Design Iterations (except for ROM & TESTS apps).
            FOREACH(hw_di ${app_hw_di_list})
                # Add a HW Design Iterations suffix (except for ROM & TESTS apps)
                IF(APP_IS_IN_ROM OR APP_IS_TEST)
                    SET(build_hw_di_postfix "")
                ELSE()
                    SET(build_hw_di_postfix "_${hw_di}")
                ENDIF()
                # Add the executable.
                IF((APP_IS_IN_ROM OR APP_IS_IN_NVM OR APP_IS_STANDALONE OR APP_IS_USING_EMCORE) AND build_all_target)
                    IF(NOT SDK_BUILD)
                        ADD_EXECUTABLE(${build_target}${build_hw_di_postfix} ${TARGET_SRCS})
                    ELSE()
                        ADD_EXECUTABLE(${build_target}${build_hw_di_postfix} EXCLUDE_FROM_ALL ${TARGET_SRCS})
                        ADD_DEPENDENCIES(all_${hw_di} ${build_target}${build_hw_di_postfix})
                    ENDIF()
                    SET_TARGET_PROPERTIES(${build_target}${build_hw_di_postfix}
                        PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                        )
                    #ADD_CUSTOM_COMMAND(${build_target}${build_hw_di_postfix}
                    #    COMMAND )
                    IF(${hw_di} STREQUAL ${DEFAULT_DI})
                        IF (BUILD_SDK_INSTALLER)
                            ADD_CUSTOM_TARGET(${build_target} ALL ${CMAKE_COMMAND} -E copy "${build_target}${build_hw_di_postfix}.ihex" "${build_target}.ihex"
                                COMMAND ${CMAKE_COMMAND} -E copy "${build_target}${build_hw_di_postfix}.asm" "${build_target}.asm"
                                COMMAND ${CMAKE_COMMAND} -E copy "${build_target}${build_hw_di_postfix}.sym" "${build_target}.sym"
                                COMMAND ${CMAKE_COMMAND} -E copy "${build_target}${build_hw_di_postfix}.elf" "${build_target}.elf"
                                COMMAND ${CMAKE_COMMAND} -E copy "${build_target}${build_hw_di_postfix}.map" "${build_target}.map"
                                COMMAND ${CMAKE_COMMAND} -E copy "${build_target}${build_hw_di_postfix}_valid_hdr.elf" "${build_target}_valid_hdr.elf"

                                DEPENDS ${build_target}${build_hw_di_postfix}
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                COMMENT "Generating ${build_target} for ${DEFAULT_DI}"
                                )
                        ELSE()
                            ADD_CUSTOM_TARGET(${build_target} ALL ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}.ihex" "${build_target}.ihex"
                                COMMAND ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}.asm" "${build_target}.asm"
                                COMMAND ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}.sym" "${build_target}.sym"
                                COMMAND ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}.elf" "${build_target}.elf"
                                COMMAND ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}.map" "${build_target}.map"
                                COMMAND ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}_valid_hdr.elf" "${build_target}_valid_hdr.elf"

                                DEPENDS ${build_target}${build_hw_di_postfix}
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                COMMENT "Generating ${build_target}"
                                )
                        ENDIF()
                        IF(NOT SDK_BUILD)
                            ADD_CUSTOM_COMMAND(TARGET ${build_target}
                                COMMAND ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}_meta.txt" "${build_target}_meta.txt"
                                COMMAND ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}_nvm_main.hex" "${build_target}_nvm_main.hex"
                                COMMAND ${CMAKE_COMMAND} -E rename "${build_target}${build_hw_di_postfix}_nvm_info.hex" "${build_target}_nvm_info.hex"
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                # COMMENT "Generating ${build_target} for ${DEFAULT_DI}"
                                )
                        ENDIF()
                    ENDIF()
                    IF(APP_IS_EMCORE)
                        # MESSAGE("ADD Target ${build_target}_no_crc_check${build_hw_di_postfix}")
                        ADD_EXECUTABLE(${build_target}_no_crc_check${build_hw_di_postfix} ${TARGET_SRCS})
                    ENDIF()
                ELSE()
                    ADD_EXECUTABLE(${build_target}${build_hw_di_postfix} EXCLUDE_FROM_ALL ${TARGET_SRCS})
                ENDIF()
                IF(APP_IS_USING_EMCORE)
                    SET_TARGET_PROPERTIES(${build_target}${build_hw_di_postfix} PROPERTIES EMCORE_VERSION "${APP_EMCORE_VERSION}")
                    SET_TARGET_PROPERTIES(${build_target}${build_hw_di_postfix} PROPERTIES EMCORE_FLAVOR "${APP_EMCORE_FLAVOR}")

                ENDIF()
                SET_TARGET_PROPERTIES(${build_target}${build_hw_di_postfix} PROPERTIES APP_IS_BOOTLOADER "${APP_IS_BOOTLOADER}")
                SET_TARGET_PROPERTIES(${build_target}${build_hw_di_postfix} PROPERTIES APP_IS_FIRMWARE_UPDATER "${APP_IS_FIRMWARE_UPDATER}")

                # Add all TEST apps in the 'tests' target.
                IF(APP_IS_TEST)
                    ADD_DEPENDENCIES(tests ${build_target}${build_hw_di_postfix})
                ENDIF()

                # Use the Source Clean tool to clean the source code (except in SDK).
                IF(NOT SDK_BUILD)
                    ADD_DEPENDENCIES(${build_target}${build_hw_di_postfix} source_clean)
                ENDIF()

                # Add dependency to nvm_bootloader_base target into monolithic application binaries.
                # EM-Core apps require NVM-Bootloader to be written separately to NVM.
                IF(APP_IS_IN_NVM AND INCLUDE_NVMBL_IN_IHEX)
                    ADD_DEPENDENCIES(${build_target}${build_hw_di_postfix} nvm_bootloader_base${build_hw_di_postfix})
                ENDIF()

                #IF(APP_IS_EMCORE AND NOT CUSTOM_EMCORE)
                IF(APP_IS_EMCORE)
                    # Add dependency here to make sure the no_crc_check is built first
                    # in order to install both, legacy and no_crc_check ihex file at once.
                    ADD_DEPENDENCIES(${build_target}${build_hw_di_postfix} ${build_target}_no_crc_check${build_hw_di_postfix})
                ENDIF()
                # Add a compilation flag with the selected HW Design Iteration.
                LIST(FIND HW_DI_INDEX "${hw_di}" hw_di_idx)
                SET(build_hw_di_flag "-DHW_DI=${hw_di_idx}")

                # Add the common compilation flags according to the build config and application location.
                ADD_TARGET_COMPILE_FLAGS(${build_target}${build_hw_di_postfix} "${build_cflags} ${build_hw_di_flag} ${APP_CFLAGS}")
                ADD_TARGET_LD_FLAGS(${build_target}${build_hw_di_postfix} "${APP_LDFLAGS}")

                ## Check if there is a private_symbols_xxx_yyy.sym file in the current source folder for the current DI.
                ## As an example, if a NVM sample application folder contains private_symbols_di05.sym, it will be automatically added to the
                ## Linker command. This enables the use of ROM symbols not exposed in the rom.sym file for e.g., unit tests.
                IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/private_symbols${build_postfix}${build_hw_di_postfix}.sym)
                    # MESSAGE("Adding extra private symbols from ${CMAKE_CURRENT_SOURCE_DIR}/private_symbols${build_postfix}${build_hw_di_postfix}.sym to the target ${build_target}${build_hw_di_postfix}")
                    ADD_TARGET_LD_FLAGS(${build_target}${build_hw_di_postfix} "-Bsymin=${CMAKE_CURRENT_SOURCE_DIR}/private_symbols${build_postfix}${build_hw_di_postfix}.sym")
                ENDIF()

                IF(APP_IS_EMCORE)
                    ADD_TARGET_COMPILE_FLAGS(${build_target}_no_crc_check${build_hw_di_postfix} "${build_cflags} ${build_hw_di_flag} ${APP_CFLAGS}")
                    ADD_TARGET_LD_FLAGS(${build_target}_no_crc_check${build_hw_di_postfix} "${APP_LDFLAGS}")
                ENDIF()

                # Define the linker script to be used.
                SET(LINKER_SCRIPT_FULL_PATH "${COMMON_DIR}/${TARGET_HW}/${APP_LINKER_FOLDER}/${build_linker_script}")
                SET_TARGET_PROPERTIES(${build_target}${build_hw_di_postfix} PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT_FULL_PATH}")
                ADD_TARGET_LD_FLAGS(${build_target}${build_hw_di_postfix}
                    -Wl,-A"${LINKER_SCRIPT_FULL_PATH}"
                )

                # Generate a output symbols file and a map file.
                ADD_TARGET_LD_FLAGS(${build_target}${build_hw_di_postfix}
                    -Bsymout="${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}.sym"
                    -m                                                  # Generate a map file
                    -Coutput="${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}.map"  # Redirect the map file to a file
                    )
                IF(APP_IS_EMCORE)
                    SET_TARGET_PROPERTIES(${build_target}_no_crc_check${build_hw_di_postfix} PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT_FULL_PATH}")
                    ADD_TARGET_LD_FLAGS(${build_target}_no_crc_check${build_hw_di_postfix}
                    -Wl,-A"${LINKER_SCRIPT_FULL_PATH}"
                    )

                    # Generate a output symbols file.
                    # ADD_TARGET_LD_FLAGS(${build_target}_no_crc_check${build_hw_di_postfix}
                    #     -Bsymout="${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}.sym"
                    # )
                ENDIF()
                IF(APP_IS_IN_ROM)
                    # For ROM, generate another symbols file with only the symbols that are listed in symlist.txt.
                    ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} ARGS -E copy
                            ${build_target}${build_hw_di_postfix}.sym
                            ${build_target}${build_hw_di_postfix}_full.sym
                        COMMAND ${PATH_GREP} ARGS
                            -w -f ${CMAKE_CURRENT_SOURCE_DIR}/symlist.txt -e "#version" -e "!EXE" -e "!END"
                            ${build_target}${build_hw_di_postfix}_full.sym > ${build_target}${build_hw_di_postfix}.sym
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Clean symbols file ${build_target}${build_hw_di_postfix}.sym"
                        VERBATIM
                    )
                ENDIF()

                # Link the ROM symbols for app that is not in ROM and is not a TEST.
                IF(NOT APP_IS_IN_ROM AND NOT APP_IS_TEST)
                    # The base path for the ROM symbols file depends on if we are in the ROM or in the NVM repository.
                    # If the target 'rom_dev' exists it means we are in the NVM repository.
                    IF("${ROM_VERSION_${hw_di}}" STREQUAL "dev" AND NOT TARGET rom_dev)
                        SET(ROM_SYMBOLS_BASE_PATH "${BUILD_DIR}/rom")
                    ELSE()
                        SET(ROM_SYMBOLS_BASE_PATH "${ROM_DIR}/bin/${ROM_VERSION_${hw_di}}")
                    ENDIF()

                    # Set the symbols file path and add a rule to link with it.
                    IF(APP_IS_USING_EMCORE)
                        GET_EMCORE_FLAVOR_ID(${APP_EMCORE_FLAVOR} emcore_flavor_id is_custom_emcore)
                        IF(NOT is_custom_emcore)
                            SET(SYMBOLS_PATH "${EMCORE_BIN_DIR}/${APP_EMCORE_VERSION}/${APP_EMCORE_FLAVOR}/emcore_${APP_EMCORE_FLAVOR}${build_postfix}${build_hw_di_postfix}.sym")
                        ELSE()
                            SET(SYMBOLS_PATH "${EMCORE_BIN_DIR}/${APP_EMCORE_VERSION}/${APP_EMCORE_FLAVOR}/custom_emcore_${APP_EMCORE_FLAVOR}${build_postfix}${build_hw_di_postfix}.sym")
                            IF(NOT EXISTS "${SYMBOLS_PATH}") # Will be created later
                                FILE(WRITE "${SYMBOLS_PATH}" "")
                            ENDIF()
                        ENDIF()
                    ELSEIF(APP_FULL_ROM_SYM)
                        SET(SYMBOLS_PATH "${ROM_SYMBOLS_BASE_PATH}/${ROM_TARGET}_full.sym")
                    ELSEIF(APP_IS_STANDALONE)
                        SET(SYMBOLS_PATH "${ROM_SYMBOLS_BASE_PATH}/${ROM_TARGET}.sym")
                    ELSE()
                        SET(SYMBOLS_PATH "${ROM_SYMBOLS_BASE_PATH}/${ROM_TARGET}.sym")
                    ENDIF()
                    ADD_TARGET_LD_FLAGS(${build_target}${build_hw_di_postfix}
                            -Bsymin_weak="${SYMBOLS_PATH}"
                            )
                    IF(APP_IS_EMCORE)
                        ADD_TARGET_LD_FLAGS(${build_target}_no_crc_check${build_hw_di_postfix}
                            -Bsymin_weak="${SYMBOLS_PATH}"
                            )
                    ENDIF()
                    # Force some symbols to be excluded in NVM.
                    IF(APP_IS_IN_NVM OR APP_IS_STANDALONE)
                        ADD_TARGET_LD_FLAGS(${build_target}${build_hw_di_postfix}
                            -Bsymin_exclude="${COMMON_DIR}/${TARGET_HW}/${APP_LINKER_FOLDER}/symlist_exclude.txt"
                            )
                        IF(APP_IS_EMCORE)
                            ADD_TARGET_LD_FLAGS(${build_target}_no_crc_check${build_hw_di_postfix}
                              -Bsymin_exclude="${COMMON_DIR}/${TARGET_HW}/${APP_LINKER_FOLDER}/symlist_exclude.txt"
                              )
                        ENDIF()
                    ENDIF()

                    SET_TARGET_PROPERTIES(${build_target}${build_hw_di_postfix} PROPERTIES LINK_DEPENDS "${SYMBOLS_PATH}")
                    IF(APP_IS_EMCORE)
                        SET_TARGET_PROPERTIES(${build_target}_no_crc_check${build_hw_di_postfix} PROPERTIES LINK_DEPENDS "${SYMBOLS_PATH}")
                    ENDIF()
                    IF(APP_IS_USING_EMCORE)
                        IF(SDK_BUILD AND NOT EXISTS ${SYMBOLS_PATH} AND NOT ${APP_IS_USING_CUSTOM_EMCORE})
                            MESSAGE(FATAL_ERROR "'emcore_${APP_EMCORE_FLAVOR}${build_postfix}${build_hw_di_postfix}.sym' file not found!")
                        ELSEIF(SDK_BUILD AND NOT EXISTS ${SYMBOLS_PATH} AND ${APP_IS_USING_CUSTOM_EMCORE})
                            MESSAGE("'custom_emcore_${APP_EMCORE_FLAVOR}${build_postfix}${build_hw_di_postfix}.sym' file not found! Pleae build it first!")
                        ENDIF()
                    ELSEIF("${ROM_VERSION_${hw_di}}" STREQUAL "dev")
                        # Add a dependency to the development ROM.
                        IF(TARGET rom_dev)
                            ADD_DEPENDENCIES(${build_target}${build_hw_di_postfix} rom_dev-${ROM_TARGET})
                        ELSE()
                            ADD_DEPENDENCIES(${build_target}${build_hw_di_postfix} ${ROM_TARGET})
                            IF(APP_IS_EMCORE)
                                ADD_DEPENDENCIES(${build_target}_no_crc_check${build_hw_di_postfix} ${ROM_TARGET})
                            ENDIF()
                        ENDIF()
                    ELSE()
                        # Check if the symbol file exists.
                        IF(NOT EXISTS ${SYMBOLS_PATH})
                            MESSAGE(FATAL_ERROR "'${ROM_TARGET}.sym' file not found for the ROM '${ROM_VERSION_${hw_di}}'!")
                        ENDIF()
                    ENDIF()

                    # Avoid errors with the Ninja generator.
                    IF(NOT EXISTS "${SYMBOLS_PATH}")
                        FILE(WRITE "${SYMBOLS_PATH}" "")
                    ENDIF()
                ENDIF()

                # Add the jli.h header as a dependency for all source files.
                GET_TARGET_PROPERTY(all_sources ${build_target}${build_hw_di_postfix} SOURCES)
                SET_SOURCE_FILES_PROPERTIES(${all_sources} PROPERTIES
                    OBJECT_DEPENDS "${COMMON_DIR}/${TARGET_HW}/includes/jli.h"
                )

                # Automatically link the Test libraray for TEST apps.
                IF(APP_IS_TEST)
                    IF(TARGET Test${build_postfix}${build_hw_di_postfix})
                        TARGET_LINK_LIBRARIES(${build_target}${build_hw_di_postfix} PRIVATE Test${build_postfix}${build_hw_di_postfix})
                    ELSE()
                        TARGET_LINK_LIBRARIES(${build_target}${build_hw_di_postfix} PRIVATE Test${build_postfix})
                    ENDIF()
                ENDIF()

                # Automatically link the app_entry libraray for NVM apps that use EMCORE.
                IF(APP_IS_USING_EMCORE)
                    IF(TARGET app_entry${build_postfix}${build_hw_di_postfix})
                        TARGET_LINK_LIBRARIES(${build_target}${build_hw_di_postfix} PRIVATE app_entry${build_postfix}${build_hw_di_postfix})
                    ELSE()
                        TARGET_LINK_LIBRARIES(${build_target}${build_hw_di_postfix} PRIVATE app_entry${build_postfix})
                    ENDIF()
                ENDIF()

                # Add a dependancy to the required emcore target.
                IF (APP_IS_USING_EMCORE)
                    IF(NOT SDK_BUILD AND TARGET emcore_${APP_EMCORE_FLAVOR}${build_postfix}${build_hw_di_postfix})
                        ADD_DEPENDENCIES(${build_target}${build_hw_di_postfix} emcore_${APP_EMCORE_FLAVOR}${build_postfix}${build_hw_di_postfix})
                    ELSEIF(${APP_IS_USING_CUSTOM_EMCORE} AND TARGET custom_emcore_${APP_EMCORE_FLAVOR}${build_postfix}${build_hw_di_postfix})
                        ADD_DEPENDENCIES(${build_target}${build_hw_di_postfix} custom_emcore_${APP_EMCORE_FLAVOR}${build_postfix}${build_hw_di_postfix})
                    ENDIF()
                ENDIF()

                # Check whether or not this target requires a firmware header.
                IF((APP_IS_USING_EMCORE OR APP_IS_IN_NVM OR APP_IS_BOOTLOADER)
                        AND NOT APP_IS_NVMBOOTLOADER)
                    # Set required parameters for building header.
                    SET(_segment_type "monolithic")
                    SET(_emcore_flavor "n/a")
                    SET(_emcore_version "n/a")
                    IF(BUILD_SDK_INSTALLER)
                        IF("${SDK_TYPE}" STREQUAL "sdk-aw")
                            GIT_DESCRIBE("sdk" SDK_VERSION "${ROOT_DIR}")
                        ELSE()
                            GIT_DESCRIBE("sdk-emb" SDK_VERSION "${ROOT_DIR}")
                        ENDIF()
                        string(REGEX REPLACE "v(.)\.(.)(.*)" "0x0\\10\\2" _hdr_sdk_version ${SDK_VERSION})
                        SET(_firmware_version ${_hdr_sdk_version})
                    ELSE()
                        SET(_firmware_version ${APP_SEMANTIC_VERSION})
                    ENDIF()

                    IF(APP_IS_USING_EMCORE)
                        SET(_segment_type "application")
                        SET(_emcore_flavor ${APP_EMCORE_FLAVOR})
                        SET(_emcore_version ${APP_EMCORE_VERSION})
                    ELSEIF(APP_IS_EMCORE)
                        SET(_segment_type "emcore")
                        SET(_emcore_flavor ${APP_EMCORE_FLAVOR})
                        SET(_emcore_version ${APP_EMCORE_VERSION})
                        string(REGEX REPLACE "v(.)\.(.)(.*)" "0x0\\10\\2" _hdr_emcore_version ${APP_EMCORE_VERSION})
                        SET(_firmware_version ${_hdr_emcore_version})
                    ELSEIF(APP_IS_BOOTLOADER)
                        SET(_segment_type "bootloader")
                    ELSEIF(APP_IS_FIRMWARE_UPDATER)
                        SET(_segment_type "firmware_updater")
                    ENDIF()
                    IF(APP_IS_EMCORE)
                        SET(IMAGE_CRC_BACKUP ${IMAGE_CRC_CHECK})
                        SET(IMAGE_CRC_CHECK TRUE)
                    ENDIF()
                    ADD_HEADER_TO_TARGET(${build_target}${build_hw_di_postfix}
                        ${_segment_type} ${_emcore_flavor}
                        ${_firmware_version})
                    IF(APP_IS_EMCORE)
                        SET(IMAGE_CRC_CHECK FALSE)
                        ADD_HEADER_TO_TARGET(${build_target}_no_crc_check${build_hw_di_postfix}
                            ${_segment_type} ${_emcore_flavor}
                            ${_firmware_version})
                        SET(IMAGE_CRC_CHECK ${IMAGE_CRC_BACKUP})
                    ENDIF()
                ENDIF()

                SET(HEADERSTUFFER_ARGS ${build_target}${build_hw_di_postfix}.elf)

                # Generate a hex file in Intel hex format.
                ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                    COMMAND ${PATH_HEADERSTUFFER} ${HEADERSTUFFER_ARGS} && ${CMAKE_PREFIX_PATH}elf2hex -QIo ${build_target}${build_hw_di_postfix}.ihex ${build_target}${build_hw_di_postfix}_valid_hdr.elf
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating ${build_target}${build_hw_di_postfix}.ihex" VERBATIM
                )
                # Concat the nvm_bootloader_base ihex to the generated ihex for the monolithic applications
                IF(APP_IS_IN_NVM AND INCLUDE_NVMBL_IN_IHEX)
                    ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                    COMMAND ${PATH_HEAD} -n -1 ${ROOT_DIR}/emcore/bin/nvm_bootloader/nvm_bootloader_base${build_hw_di_postfix}.ihex > ${build_target}${build_hw_di_postfix}_temp.ihex
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    )
                    ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                    COMMAND ${PATH_TAIL} -n +2 ${build_target}${build_hw_di_postfix}.ihex >> ${build_target}${build_hw_di_postfix}_temp.ihex
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    )
                    ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                    COMMAND ${PATH_MV} ${build_target}${build_hw_di_postfix}_temp.ihex ${build_target}${build_hw_di_postfix}.ihex
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Adding nvm_bootloader_base${build_hw_di_postfix}.ihex to ${build_target}${build_hw_di_postfix}.ihex" VERBATIM
                    )
                ENDIF()
                IF(APP_IS_EMCORE)
                    SET(HEADERSTUFFER_ARGS ${build_target}_no_crc_check${build_hw_di_postfix}.elf)
                    ADD_CUSTOM_COMMAND(TARGET ${build_target}_no_crc_check${build_hw_di_postfix} POST_BUILD
                        COMMAND ${PATH_HEADERSTUFFER} ${HEADERSTUFFER_ARGS} && ${CMAKE_PREFIX_PATH}elf2hex -QIo ${build_target}_no_crc_check${build_hw_di_postfix}.ihex ${build_target}_no_crc_check${build_hw_di_postfix}_valid_hdr.elf
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating ${build_target}_no_crc_check${build_hw_di_postfix}.ihex" VERBATIM
                )
                ENDIF()
                IF(NOT SDK_BUILD)
                    # Output files generation for IRAM apps.
                    IF(APP_IS_IN_IRAM)
                        # Generate the IRAM HEX.
                        ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                            COMMAND ${PATH_ELF2MIF} ARGS
                                ${build_target}${build_hw_di_postfix}.elf
                                ${build_target}${build_hw_di_postfix}.mif
                                ${HW_IRAM_TESTCODE_ADDR} ${HW_IRAM_TESTCODE_SIZE}
                            COMMAND ${PATH_MIF2HEX} ARGS
                                ${build_target}${build_hw_di_postfix}.mif
                                ${build_target}${build_hw_di_postfix}.hex
                            COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                ${build_target}${build_hw_di_postfix}.mif
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                            COMMENT "Generating ${build_target}${build_hw_di_postfix}.hex"
                            VERBATIM
                        )

                    # Output files generation for ROM, NVM, and TEST apps.
                    ELSE()
                        SET(METADATA_FILE "${build_target}${build_hw_di_postfix}_meta.txt")
                        SET(METADATA_REDIRECT ">" ${METADATA_FILE})
                        IF(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
                            SET(PRINT_METADATA "type" ${METADATA_FILE})
                        ELSE()
                            SET(PRINT_METADATA "cat" ${METADATA_FILE})
                        ENDIF()
                        IF(APP_IS_IN_ROM OR APP_IS_TEST)
                            # Generate the ROM HEX.
                            ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                COMMAND ${PATH_ELF2MIF} ARGS
                                    ${build_target}${build_hw_di_postfix}.elf
                                    ${build_target}${build_hw_di_postfix}_rom.mif
                                    ${HW_IROM_ADDR} ${build_irom_size}
                                    ${METADATA_REDIRECT}
                                COMMAND ${PATH_MIF2HEX} ARGS
                                    ${build_target}${build_hw_di_postfix}_rom.mif
                                    ${build_target}${build_hw_di_postfix}_rom.hex
                                    ${METADATA_REDIRECT}
                                COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                    ${build_target}${build_hw_di_postfix}_rom.mif
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                COMMENT "Generating ${build_target}${build_hw_di_postfix}_rom.hex"
                                VERBATIM
                            )
                        ENDIF()

                        # Generate the NVM Main HEX.
                        ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                            COMMAND ${PATH_ELF2MIF} ARGS
                                ${build_target}${build_hw_di_postfix}_valid_hdr.elf
                                ${build_target}${build_hw_di_postfix}_nvm_main.mif
                                ${HW_NVM_ADDR} ${HW_NVM_SIZE} --64bits --fillByOnes
                            COMMAND ${PATH_MIF2HEX} ARGS
                                ${build_target}${build_hw_di_postfix}_nvm_main.mif
                                ${build_target}${build_hw_di_postfix}_nvm_main.hex
                            COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                ${build_target}${build_hw_di_postfix}_nvm_main.mif
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                            COMMENT "Generating ${build_target}${build_hw_di_postfix}_nvm_main.hex"
                            VERBATIM
                        )

                        # Generate the NVM Info HEX.
                        ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                            COMMAND ${PATH_ELF2MIF} ARGS
                                ${build_target}${build_hw_di_postfix}.elf
                                ${build_target}${build_hw_di_postfix}_nvm_info.mif
                                ${HW_NVM_INFO_ADDR} ${HW_NVM_INFO_SIZE} --64bits --fillByOnes
                            COMMAND ${PATH_MIF2HEX} ARGS
                                ${build_target}${build_hw_di_postfix}_nvm_info.mif
                                ${build_target}${build_hw_di_postfix}_nvm_info.hex
                            COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                ${build_target}${build_hw_di_postfix}_nvm_info.mif
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                            COMMENT "Generating ${build_target}${build_hw_di_postfix}_nvm_info.hex"
                            VERBATIM
                        )
                    ENDIF()

                    # Generate bitstream if required and enabled.
                    IF(NOT SDK_BUILD AND build_bitstream AND NOT APP_IS_IN_IRAM AND
                        ((NOT APP_IS_IN_NVM AND NOT APP_IS_STANDALONE) OR GENERATE_NVM_BITSTREAM) AND NOT APP_IS_USING_EMCORE AND
                        (GENERATE_FPGA_ARTIX_BITSTREAM OR GENERATE_FPGA_ZYNQ_BITSTREAM))

                        # Bitstream generation needs to have .mem extension so copy hex files.
                        IF(APP_IS_IN_ROM OR APP_IS_TEST)
                            ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                COMMAND ${CMAKE_COMMAND} ARGS -E copy
                                    ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_rom.hex
                                    ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_rom.mem
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                VERBATIM
                            )
                        ENDIF()
                        ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} ARGS -E copy
                                ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_main.hex
                                ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_main.mem
                            COMMAND ${CMAKE_COMMAND} ARGS -E copy
                                ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_info.hex
                                ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_info.mem
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                            VERBATIM
                        )

                        IF(APP_IS_IN_ROM OR APP_IS_TEST)
                            SET(step_current    1)
                            SET(step_total      3)

                            SET(rom_bitstream_suffix    ${build_target}${build_hw_di_postfix})

                            # Integrating ROM memory into FPGA bitstream.
                            IF(GENERATE_FPGA_ARTIX_BITSTREAM)
                                ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                    COMMAND ${PATH_DATA2MEM} ARGS
                                        -bm ${build_bitstream_path}/artix/t9305_fpga_rom.bmm
                                        -bd ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_rom.mem tag ICCM_ROM
                                        -bt ${build_bitstream_path}/artix/t9305_fpga.bit
                                        -o b ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${rom_bitstream_suffix}.bit
                                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                    COMMENT "Generating FPGA Artix bitstream (step ${step_current}/${step_total})"
                                    VERBATIM
                                )
                            ENDIF()
                            IF(GENERATE_FPGA_ZYNQ_BITSTREAM)
                                ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                    COMMAND ${PATH_XILINX_XSCT} ARGS
                                        ${PATH_XSCT_UPDATEMEM}
                                        ${build_bitstream_path}/zynq/t9305_fpga.bit
                                        ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ZYNQ_FILENAME}_${rom_bitstream_suffix}.bit
                                        ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_rom.mem
                                        ${build_bitstream_path}/zynq/t9305_fpga_rom.mmi
                                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                    COMMENT "Generating FPGA Zynq bitstream (step ${step_current}/${step_total})"
                                    VERBATIM
                                )
                            ENDIF()
                            ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                    ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_rom.mem
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                VERBATIM
                            )
                            MATH(EXPR step_current "${step_current} + 1")

                        ELSE()
                            SET(step_current    1)
                            SET(step_total      2)

                            SET(rom_bitstream_suffix    "${ROM_TARGET}")

                            IF(GENERATE_FPGA_ARTIX_BITSTREAM)
                                SET(FPGA_ARTIX_FILENAME "t9305_fpga_artix_tmp_${build_target}${build_hw_di_postfix}")
                                ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                    COMMAND ${CMAKE_COMMAND} ARGS
                                        -Dfpga=artix
                                        -Dversion=${ROM_VERSION_${hw_di}}
                                        -Dpath=${ROM_DIR}/bin/${ROM_VERSION_${hw_di}}
                                        -Dsuffix=${rom_bitstream_suffix}
                                        -Dname=${FPGA_ARTIX_FILENAME}
                                        -P ${CMAKE_DIR}/scripts/copy_rom_bitstream.cmake
                                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                    VERBATIM
                                )
                            ENDIF()
                            IF(GENERATE_FPGA_ZYNQ_BITSTREAM)
                                SET(FPGA_ZYNQ_FILENAME "t9305_fpga_zynq_tmp_${build_target}${build_hw_di_postfix}")
                                ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                    COMMAND ${CMAKE_COMMAND} ARGS
                                        -Dfpga=zynq
                                        -Dversion=${ROM_VERSION_${hw_di}}
                                        -Dpath=${ROM_DIR}/bin/${ROM_VERSION_${hw_di}}
                                        -Dsuffix=${rom_bitstream_suffix}
                                        -Dname=${FPGA_ZYNQ_FILENAME}
                                        -P ${CMAKE_DIR}/scripts/copy_rom_bitstream.cmake
                                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                    VERBATIM
                                )
                            ENDIF()
                        ENDIF()

                        # Integrating NVM Main memory into FPGA bitstream.
                        IF(GENERATE_FPGA_ARTIX_BITSTREAM)
                            ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                COMMAND ${CMAKE_COMMAND} ARGS -E copy
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${rom_bitstream_suffix}.bit
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${rom_bitstream_suffix}_tmp.bit
                                COMMAND ${PATH_DATA2MEM} ARGS
                                    -bm ${build_bitstream_path}/artix/t9305_fpga_nvm_main.bmm
                                    -bd ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_main.mem tag NVM_MAIN
                                    -bt ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${rom_bitstream_suffix}_tmp.bit
                                    -o b ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${build_target}${build_hw_di_postfix}.bit
                                COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${rom_bitstream_suffix}_tmp.bit
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                COMMENT "Generating FPGA Artix bitstream (step ${step_current}/${step_total})"
                                VERBATIM
                            )
                        ENDIF()
                        IF(GENERATE_FPGA_ZYNQ_BITSTREAM)
                            ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                COMMAND ${PATH_XILINX_XSCT} ARGS
                                    ${PATH_XSCT_UPDATEMEM}
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ZYNQ_FILENAME}_${rom_bitstream_suffix}.bit
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ZYNQ_FILENAME}_${build_target}${build_hw_di_postfix}.bit
                                    ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_main.mem
                                    ${build_bitstream_path}/zynq/t9305_fpga_nvm_main.mmi
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                COMMENT "Generating FPGA Zynq bitstream (step ${step_current}/${step_total})"
                                VERBATIM
                            )
                        ENDIF()
                        ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_main.mem
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                            VERBATIM
                        )
                        MATH(EXPR step_current "${step_current} + 1")

                        # Integrating NVM Info memory into FPGA bitstream.
                        IF(GENERATE_FPGA_ARTIX_BITSTREAM)
                            ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                COMMAND ${CMAKE_COMMAND} ARGS -E copy
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${build_target}${build_hw_di_postfix}.bit
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${build_target}${build_hw_di_postfix}_tmp.bit
                                COMMAND ${PATH_DATA2MEM} ARGS
                                    -bm ${build_bitstream_path}/artix/t9305_fpga_nvm_info.bmm
                                    -bd ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_info.mem tag NVM_INFO
                                    -bt ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${build_target}${build_hw_di_postfix}_tmp.bit
                                    -o b ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${build_target}${build_hw_di_postfix}.bit
                                COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ARTIX_FILENAME}_${build_target}${build_hw_di_postfix}_tmp.bit
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                COMMENT "Generating FPGA Artix bitstream (step ${step_current}/${step_total})"
                                VERBATIM
                            )
                        ENDIF()
                        IF(GENERATE_FPGA_ZYNQ_BITSTREAM)
                            ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                COMMAND ${PATH_XILINX_XSCT} ARGS
                                    ${PATH_XSCT_UPDATEMEM}
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ZYNQ_FILENAME}_${build_target}${build_hw_di_postfix}.bit
                                    ${CMAKE_CURRENT_BINARY_DIR}/${FPGA_ZYNQ_FILENAME}_${build_target}${build_hw_di_postfix}.bit
                                    ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_info.mem
                                    ${build_bitstream_path}/zynq/t9305_fpga_nvm_info.mmi
                                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                COMMENT "Generating FPGA Zynq bitstream (step ${step_current}/${step_total})"
                                VERBATIM
                            )
                        ENDIF()
                        ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} ARGS -E remove -f
                                ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}_nvm_info.mem
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                            VERBATIM
                        )
                        MATH(EXPR step_current "${step_current} + 1")

                        IF(NOT APP_IS_IN_ROM AND NOT APP_IS_TEST)
                            IF(GENERATE_FPGA_ARTIX_BITSTREAM)
                                ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                    COMMAND ${CMAKE_COMMAND} ARGS
                                        -Dfpga=artix
                                        -Dversion=${ROM_VERSION_${hw_di}}
                                        -Dpath=${ROM_DIR}/bin/${ROM_VERSION_${hw_di}}
                                        -Dsuffix=${rom_bitstream_suffix}
                                        -Dname=${FPGA_ARTIX_FILENAME}
                                        -Dbuild_target=${build_target}${build_hw_di_postfix}
                                        -P ${CMAKE_DIR}/scripts/rename_tmp_bitstream.cmake
                                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                    VERBATIM
                                )
                            ENDIF()
                            IF(GENERATE_FPGA_ZYNQ_BITSTREAM)
                                ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                                    COMMAND ${CMAKE_COMMAND} ARGS
                                        -Dfpga=zynq
                                        -Dversion=${ROM_VERSION_${hw_di}}
                                        -Dpath=${ROM_DIR}/bin/${ROM_VERSION_${hw_di}}
                                        -Dsuffix=${rom_bitstream_suffix}
                                        -Dname=${FPGA_ZYNQ_FILENAME}
                                        -Dbuild_target=${build_target}${build_hw_di_postfix}
                                        -P ${CMAKE_DIR}/scripts/rename_tmp_bitstream.cmake
                                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                                    VERBATIM
                                )
                            ENDIF()
                        ENDIF()
                    ENDIF()
                ENDIF()

                # Generate the assembly dump if required.
                IF(GENERATE_ASSEMBLY_DUMP)
                    IF(NOT SDK_BUILD AND INLINE_CODE_IN_ASSEMBLY_DUMP)
                        SET(ELFDUMP_SCRIPT ${CMAKE_DIR}/scripts/elfdump_intermixed.cmake)
                    ELSE()
                        SET(ELFDUMP_SCRIPT ${CMAKE_DIR}/scripts/elfdump.cmake)
                    ENDIF()
                    ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} ARGS
                            -DIN=${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}.elf
                            -DOUT=${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}.asm
                            -P ${ELFDUMP_SCRIPT}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating ${build_target}${build_hw_di_postfix}.asm"
                        VERBATIM
                    )
                ENDIF()

                # Print executable stats.
                ADD_CUSTOM_COMMAND(TARGET ${build_target}${build_hw_di_postfix} POST_BUILD
                    COMMAND ${PATH_ELFSTAT} ARGS
                        ${CMAKE_CURRENT_BINARY_DIR}/${build_target}${build_hw_di_postfix}.elf --brief
                        ${METADATA_REDIRECT}
                    COMMAND ${PRINT_METADATA}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Memory usage for ${build_target}${build_hw_di_postfix}.elf"
                    VERBATIM
                )
            ENDFOREACH()
        ENDIF()
    ENDFOREACH()

    # Be sure the defined location can be used only once.
    APP_CLEAR_LOCATION()
ENDMACRO(ARC_EXECUTABLE)
