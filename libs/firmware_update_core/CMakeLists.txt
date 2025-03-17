ADD_LIBRARY(firmware_update_core STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(firmware_update_core PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_firmware_update_core.a")

ADD_LIBRARY(firmware_update_core_di03 STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(firmware_update_core_di03 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_firmware_update_core_di03.a")

