ADD_LIBRARY(firmware_update_crypto STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(firmware_update_crypto PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_firmware_update_crypto.a")

ADD_LIBRARY(firmware_update_crypto_di03 STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(firmware_update_crypto_di03 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_firmware_update_crypto_di03.a")

