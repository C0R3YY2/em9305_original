ADD_LIBRARY(em_system STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(em_system PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_em_system.a")

ADD_LIBRARY(em_system_di03 STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(em_system_di03 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_em_system_di03.a")

ADD_LIBRARY(em_system_di04 STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(em_system_di04 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_em_system_di04.a")

ADD_LIBRARY(em_system_bl STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(em_system_bl PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_em_system_bl.a")

ADD_LIBRARY(em_system_emcore STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(em_system_emcore PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_em_system_emcore.a")

ADD_LIBRARY(em_system_emcore_di03 STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(em_system_emcore_di03 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_em_system_emcore_di03.a")

ADD_LIBRARY(em_system_emcore_di04 STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(em_system_emcore_di04 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_em_system_emcore_di04.a")

