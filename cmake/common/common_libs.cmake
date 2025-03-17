# Wrappers that are processed by the macro ARC_LINK_LIBRARIES.
SET(NVM_COMMON_LIBS             "__NVM_COMMON_LIBS__")
SET(NVM_VALIDATION_COMMON_LIBS  "__NVM_VALIDATION_COMMON_LIBS__")
SET(NVM_UNITTEST_COMMON_LIBS    "__NVM_UNITTEST_COMMON_LIBS__")


# NVM common libraries.
SET(__NVM_COMMON_LIBS__
    Metaware
    em_core
    header_access
    em_hw_api
    em_system
    nvm_entry
    pml
    radio
    aoad
    prot_timer
    sleep_timer
    rc_calib
    sleep_manager
    transport
)
SET(__NVM_COMMON_LIBS__di01
    ${__NVM_COMMON_LIBS__}
    memory_manager
    dma
    gpio
    spi_slave_dma
    uart_dma
    nvm_info
    hw_security
    nvm_fix
    ${LIBS_DIR}/security/AES_CBC_ARC.a
    ${LIBS_DIR}/security/AES_CCM_ARC.a
    ${LIBS_DIR}/security/AES_CMAC_ARC.a
    ${LIBS_DIR}/security/AES_CTR_ARC.a
    ${LIBS_DIR}/security/AES_ECB_ARC.a
    ${LIBS_DIR}/security/AES_GCM_ARC.a
    ${LIBS_DIR}/security/ECC_ARC.a
    ${LIBS_DIR}/security/PRNG_ARC.a
)
SET(__NVM_COMMON_LIBS__di03
    ${__NVM_COMMON_LIBS__di01}
)
SET(__NVM_COMMON_LIBS__di04
    ${__NVM_COMMON_LIBS__}
    gpio
    nvm_fix
)
SET(__NVM_COMMON_LIBS__di05
    ${__NVM_COMMON_LIBS__}
    nvm_fix
)

# NVM Validation common libraries.
SET(__NVM_VALIDATION_COMMON_LIBS__)
FOREACH(lib ${__NVM_COMMON_LIBS__})
    # Copy all the common libraries except the radio and PML one.
    IF(NOT ((${lib} STREQUAL "radio") OR (${lib} STREQUAL "pml")))
        SET(__NVM_VALIDATION_COMMON_LIBS__
            ${__NVM_VALIDATION_COMMON_LIBS__}
            ${lib}
        )
    ENDIF()
ENDFOREACH()
SET(__NVM_VALIDATION_COMMON_LIBS__
    ${__NVM_VALIDATION_COMMON_LIBS__}
    # Use different radio library to debug radio configuration
    radio_debug_config
    # Use different PML library to debug power configuration
    pml_debug_power
)
SET(__NVM_VALIDATION_COMMON_LIBS__di01
    ${__NVM_VALIDATION_COMMON_LIBS__}
    memory_manager
    dma
    gpio
    spi_slave_dma
    uart_dma
    nvm_info
    hw_security
    ${LIBS_DIR}/security/AES_CBC_ARC.a
    ${LIBS_DIR}/security/AES_CCM_ARC.a
    ${LIBS_DIR}/security/AES_CMAC_ARC.a
    ${LIBS_DIR}/security/AES_CTR_ARC.a
    ${LIBS_DIR}/security/AES_ECB_ARC.a
    ${LIBS_DIR}/security/AES_GCM_ARC.a
    ${LIBS_DIR}/security/ECC_ARC.a
    ${LIBS_DIR}/security/PRNG_ARC.a
    nvm_fix
)
SET(__NVM_VALIDATION_COMMON_LIBS__di03
    ${__NVM_VALIDATION_COMMON_LIBS__di01}
)
SET(__NVM_VALIDATION_COMMON_LIBS__di04
    ${__NVM_VALIDATION_COMMON_LIBS__}
    gpio
    nvm_fix
)
SET(__NVM_VALIDATION_COMMON_LIBS__di05
    ${__NVM_VALIDATION_COMMON_LIBS__}
    nvm_fix
)
# NVM UnitTest common libraries.
SET(__NVM_UNITTEST_COMMON_LIBS__
    ${NVM_COMMON_LIBS}
    unit_test
    printf
)
