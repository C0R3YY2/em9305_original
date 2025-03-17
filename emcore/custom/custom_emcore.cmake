# @todo: fix flint errors
ADD_C_FLAGS(-DNO_FLINT_ERRORS)

# Define the source files.
SET(CUSTOM_EMCORE_SRCS
    source/emcore_app.c
    source/emcore_app_task.c
    source/emcore_bsp.c
    source/emcore_nvm_main.c
)


# Define the common libraries.
SET(__CUSTOM_EMCORE_LIBS_COMMON__
    Metaware
    em_core
    em_hw_api
    em_system_stack
    em_system_emcore
    header_access
    nvm_entry_emcore
    pml
    radio
    prot_timer
    sleep_timer
    rc_calib
    sleep_manager
    transport
    QPC
)
SET(__CUSTOM_EMCORE_LIBS_COMMON__di01
    ${__CUSTOM_EMCORE_LIBS_COMMON__}
    memory_manager
    dma
    gpio
    spi_slave_dma
    uart_dma
    nvm_info
    nvm_fix
    hw_security
    ${LIBS_DIR}/security/AES_CBC_ARC.a
    ${LIBS_DIR}/security/AES_CCM_ARC.a
    ${LIBS_DIR}/security/AES_CMAC_ARC.a
    ${LIBS_DIR}/security/AES_CTR_ARC.a
    ${LIBS_DIR}/security/AES_ECB_ARC.a
    ${LIBS_DIR}/security/AES_GCM_ARC.a
    ${LIBS_DIR}/security/ECC_ARC.a
    ${LIBS_DIR}/security/PRNG_ARC.a
)
SET(__CUSTOM_EMCORE_LIBS_COMMON__di03
    ${__CUSTOM_EMCORE_LIBS_COMMON__di01}
)
SET(__CUSTOM_EMCORE_LIBS_COMMON__di04
    ${__CUSTOM_EMCORE_LIBS_COMMON__}
    gpio
    nvm_fix
)
SET(__CUSTOM_EMCORE_LIBS_COMMON__di05
    ${__CUSTOM_EMCORE_LIBS_COMMON__}
    nvm_fix
)

SET(CUSTOM_EMCORE_LIBS
    emcore_information_my_flavor
    # Add the libraries you want to include to your custom EM-Core
)

# Define the common includes.
SET(CUSTOM_EMCORE_INCLUDES_COMMON
    ${COMMON_DIR}/includes
    ${COMMON_DIR}/${TARGET_HW}/includes
    includes
    ${LIBS_DIR}/third_party/Metaware/includes
    ${LIBS_DIR}/em_system_stack/includes
    ${LIBS_DIR}/em_core/includes
    ${LIBS_DIR}/em_hw_api/includes
    ${LIBS_DIR}/em_system/includes
    ${LIBS_DIR}/firmware_header/includes
    ${LIBS_DIR}/nvm_entry/includes
    ${LIBS_DIR}/pml/includes
    ${LIBS_DIR}/radio/includes
    ${LIBS_DIR}/prot_timer/includes
    ${LIBS_DIR}/sleep_timer/includes
    ${LIBS_DIR}/rc_calib/includes
    ${LIBS_DIR}/sleep_manager/includes
    ${LIBS_DIR}/transport/includes
    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/source
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em
    ${LIBS_DIR}/memory_manager/includes
    ${LIBS_DIR}/dma/includes
    ${LIBS_DIR}/gpio/includes
    ${LIBS_DIR}/spi_slave/includes
    ${LIBS_DIR}/uart/includes
    ${LIBS_DIR}/nvm_info/includes
    ${LIBS_DIR}/security/includes
)

# Define the includes for specific flavor.
SET(CUSTOM_EMCORE_INCLUDES__my_flavor
    ${CUSTOM_EMCORE_INCLUDES_COMMON}
)

# Define the includes for specific flavor.
SET(CUSTOM_EMCORE_LIBS__my_flavor
    ${CUSTOM_EMCORE_LIBS_COMMON}
    ${CUSTOM_EMCORE_LIBS}
)

# Create the emcore targets.
FOREACH(custom_emcore_flavor ${CUSTOM_EMCORE_FLAVORS})
    GET_EMCORE_FLAVOR_INFO("${custom_emcore_flavor}" custom_emcore_flavor_name custom_emcore_flavor_id)

    IF(NOT ${custom_emcore_flavor_id} EQUAL -1) # Ignore invalid IDX
        CUSTOM_EMCORE_IN_NVM(${custom_emcore_flavor_name})
        SET(APP_CFLAGS "-DNVM_CODE")
        # '-znodefswarn' flag used to be added to the linker flags when emb libs were added to a variant of EM-Core.
        # This is no longer needed, but might be needed at some point to generate customer-specific EM-Core variant.
        SET(APP_LDFLAGS "-Ball_archive -Bdefine:APP_IS_EMCORE=1")
        SET(custom_emcore_target ${PROJECT_NAME}_${custom_emcore_flavor_name})
    #    SET(IMAGE_CRC_CHECK TRUE)
        ARC_EXECUTABLE(${custom_emcore_target} ${CUSTOM_EMCORE_SRCS})
        ARC_LINK_LIBRARIES(${custom_emcore_target} "__CUSTOM_EMCORE_LIBS_COMMON__"
            ${CUSTOM_EMCORE_LIBS__${custom_emcore_flavor_name}})
        ARC_INCLUDE_DIRECTORIES(${custom_emcore_target}
            ${CUSTOM_EMCORE_INCLUDES__${custom_emcore_flavor_name}})
        FOREACH(build ${BUILD_CONFIGS})
            ARC_GET_BUILD_TARGET(${build} ${PROJECT_NAME}_${custom_emcore_flavor_name})
            FOREACH(hw_di ${HW_DI_LIST})
                SET(tgt "${build_target}_${hw_di}")
                IF(MW_FOR_EM)
                    TARGET_LINK_LIBRARIES(${tgt} PRIVATE "-Bno_all_archive")
                ELSE()
                    TARGET_LINK_LIBRARIES(${tgt} PRIVATE "-Bno_all_archive" "-Hlib=${LIBS_DIR}/third_party/Metaware/bin/2019.09" )
                ENDIF()
                SET(tgt "${build_target}_no_crc_check_${hw_di}")
                IF(MW_FOR_EM)
                    TARGET_LINK_LIBRARIES(${tgt} PRIVATE "-Bno_all_archive")
                ELSE()
                    TARGET_LINK_LIBRARIES(${tgt} PRIVATE "-Bno_all_archive" "-Hlib=${LIBS_DIR}/third_party/Metaware/bin/2019.09" )
                ENDIF()
            ENDFOREACH()
        ENDFOREACH()
    ENDIF()
ENDFOREACH()


# Installation of the emcore targets.
FOREACH(custom_emcore_flavor ${CUSTOM_EMCORE_FLAVORS})
    # CMake script to install the header files for one emcore version.
    GET_EMCORE_FLAVOR_INFO("${custom_emcore_flavor}" custom_emcore_flavor_name custom_emcore_flavor_id)
    IF(NOT ${custom_emcore_flavor_id} EQUAL -1) # Ignore invalid IDX
        SET(install_file
            "SET(dst \"${CUSTOM_EMCORE_BIN_DIR}/${custom_emcore_flavor_name}\")\n"
        )
        SET(include_dirs ${CUSTOM_EMCORE_INCLUDES__${custom_emcore_flavor_name}})
        LIST(REVERSE include_dirs)
        # This is intentional and refers to the exclusion of Metaware Library.
        FOREACH(excluded_item ${EMCORE_EXCLUDED_HEADER_FOLDERS})
            LIST(REMOVE_ITEM include_dirs ${excluded_item})
        ENDFOREACH()

        FOREACH(include_dir ${include_dirs})
            FILE(GLOB include_files LIST_DIRECTORIES FALSE RELATIVE "${include_dir}" "${include_dir}/*h")
            LIST(LENGTH include_files include_files_len)
            IF(${include_files_len} GREATER 0)
                SET(install_file ${install_file}
                    "\nSET(path \"${include_dir}\")\n"
                    "FILE(INSTALL DESTINATION \"\${dst}/includes\" TYPE FILE FILES\n"
                )
                FOREACH(include_file ${include_files})
                    SET(install_file ${install_file}
                        "    \"\${path}/${include_file}\"\n"
                    )
                ENDFOREACH()
                SET(install_file ${install_file}
                    ")\n"
                )
            ENDIF()
        ENDFOREACH()
        FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/install/install_${PROJECT_NAME}_${custom_emcore_flavor_name}_includes.cmake"
            ${install_file}
        )

        FOREACH(build ${BUILD_CONFIGS})
            ARC_GET_BUILD_TARGET(${build} ${PROJECT_NAME}_${custom_emcore_flavor_name})

            FOREACH(hw_di ${HW_DI_LIST})
                # CMake script to install one specific emcore target.
                GET_TARGET_PROPERTY(TARGET_BIN_DIR ${build_target}_${hw_di} BINARY_DIR)
                # MESSAGE("Binary dir: ${TARGET_BIN_DIR} for target ${build_target}_${hw_di})
                SET(install_file
                    "SET(dst \"${CUSTOM_EMCORE_BIN_DIR}/${custom_emcore_flavor_name}\")\n"
                    "SET(tgt \"${TARGET_BIN_DIR}\")\n"
                    "FILE(INSTALL DESTINATION \"\${dst}\" TYPE FILE FILES \"\${tgt}/${build_target}_${hw_di}.sym\")\n"
                    "FILE(INSTALL DESTINATION \"\${dst}\" TYPE FILE FILES \"\${tgt}/${build_target}_${hw_di}.ihex\")\n"
                    #"FILE(INSTALL DESTINATION \"\${dst}\" TYPE FILE FILES \"\${tgt}/${build_target}_no_crc_check_${hw_di}.ihex\")\n"
                    "FILE(INSTALL DESTINATION \"\${dst}/internal\" TYPE EXECUTABLE FILES \"\${tgt}/${build_target}_${hw_di}.elf\")\n"
                    "FILE(INSTALL DESTINATION \"\${dst}/internal\" TYPE FILE OPTIONAL FILES \"\${tgt}/${build_target}_${hw_di}.asm\")\n"
                )
                FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/install/install_${build_target}_${hw_di}.cmake"
                    ${install_file}
                )

                # Custom command to install the emcore target and the header files.
                ADD_CUSTOM_COMMAND(TARGET ${build_target}_${hw_di} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} ARGS
                        -P ./install/install_${build_target}_${hw_di}.cmake
                    COMMAND ${CMAKE_COMMAND} ARGS
                        -Dscript=./install/install_${PROJECT_NAME}_${custom_emcore_flavor_name}_includes.cmake
                        -P ${CUSTOM_EMCORE_SRC_DIR}/cmake_script_error_quiet.cmake
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} VERBATIM
                )
            ENDFOREACH()
        ENDFOREACH()
    ENDIF()
ENDFOREACH()
