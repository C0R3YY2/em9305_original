ADD_LIBRARY(firmware_update_spi STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(firmware_update_spi PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_firmware_update_spi.a")

ADD_LIBRARY(firmware_update_spi_light STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(firmware_update_spi_light PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_firmware_update_spi_light.a")

