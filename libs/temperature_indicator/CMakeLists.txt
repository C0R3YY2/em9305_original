ADD_LIBRARY(temperature_indicator STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(temperature_indicator PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_temperature_indicator.a")

