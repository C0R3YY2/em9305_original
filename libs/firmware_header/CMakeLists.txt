ADD_LIBRARY(header_access STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(header_access PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_header_access.a")

ADD_LIBRARY(header_access_di03 STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(header_access_di03 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_header_access_di03.a")

