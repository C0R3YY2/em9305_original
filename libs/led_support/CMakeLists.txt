ADD_LIBRARY(led_support STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(led_support PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_led_support.a")

