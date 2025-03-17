# Common includes.
SET(COMMON_INCLUDES
    ${COMMON_DIR}/includes
    ${COMMON_DIR}/${TARGET_HW}/includes

    ${LIBS_DIR}/third_party/Metaware/includes

    ${LIBS_DIR}/em_core/includes
    ${LIBS_DIR}/em_hw_api/includes
    ${LIBS_DIR}/em_system/includes
    ${LIBS_DIR}/firmware_header/includes

    ${LIBS_DIR}/security/includes

    ${LIBS_DIR}/unitimer/includes
    ${LIBS_DIR}/i2c/includes
    ${LIBS_DIR}/adc/includes
    ${LIBS_DIR}/spi_master/includes
    ${LIBS_DIR}/gpio/includes
    ${LIBS_DIR}/spi_slave/includes
    ${LIBS_DIR}/uart/includes
    ${LIBS_DIR}/third_party/printf
    ${LIBS_DIR}/dma/includes
    ${LIBS_DIR}/pml/includes
    ${LIBS_DIR}/radio/includes
    ${LIBS_DIR}/aoad/includes
    ${LIBS_DIR}/rtc/includes
    ${LIBS_DIR}/i2s/includes
    ${LIBS_DIR}/temperature_indicator/includes
    ${LIBS_DIR}/prot_timer/includes
    ${LIBS_DIR}/sleep_timer/includes
    ${LIBS_DIR}/rc_calib/includes
    ${LIBS_DIR}/transport/includes
    ${LIBS_DIR}/nvm_entry/includes
    ${LIBS_DIR}/nvm_info/includes
    ${LIBS_DIR}/sleep_manager/includes
    ${LIBS_DIR}/watchdog/includes
    ${LIBS_DIR}/memory_manager/includes
    ${LIBS_DIR}/nvm/includes
)

# Common includes for the ROM.
SET(ROM_COMMON_INCLUDES

    ${COMMON_DIR}/includes
    ${COMMON_DIR}/${TARGET_HW}/includes

    ${LIBS_DIR}/third_party/Metaware/includes

    ${LIBS_DIR}/em_core/includes
    ${LIBS_DIR}/em_hw_api/includes
    ${LIBS_DIR}/em_system/includes

    ${LIBS_DIR}/security/includes

    ${LIBS_DIR}/nvm/includes
    ${LIBS_DIR}/pml/includes
    ${LIBS_DIR}/memory_manager/includes
    ${LIBS_DIR}/nvm_info/includes
    ${LIBS_DIR}/spi_slave/includes
    ${LIBS_DIR}/dma/includes
    ${LIBS_DIR}/uart/includes
    ${LIBS_DIR}/transport/includes
    ${LIBS_DIR}/gpio/includes
)

# NVM UnitTest common includes.
SET(NVM_UNITTEST_COMMON_INCLUDES
    ${COMMON_INCLUDES}

    ${LIBS_DIR}/third_party/unit_test/includes
)

# NVM UnitTest common includes.
SET(ROM_UNITTEST_COMMON_INCLUDES
    ${ROM_COMMON_INCLUDES}

    ${LIBS_DIR}/nvm_entry/includes
    ${LIBS_DIR}/third_party/unit_test/includes
)

# List of header folders to exclude from output
## SDK
SET(SDK_EXCLUDED_HEADER_FOLDERS
  "${LIBS_DIR}/third_party/Metaware/includes"
)
## EMCORE
SET(EMCORE_EXCLUDED_HEADER_FOLDERS
  "${LIBS_DIR}/third_party/Metaware/includes"
)
