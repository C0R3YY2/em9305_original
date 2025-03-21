SET(UTILS_BIN_DIR "${CMAKE_DIR}/bin")

IF(CMAKE_HOST_WIN32)
    SET(PATH_CAT            "${UTILS_BIN_DIR}/cat.exe")
    SET(PATH_MV             "${UTILS_BIN_DIR}/mv.exe")
    SET(PATH_HEAD           "${UTILS_BIN_DIR}/head.exe")
    SET(PATH_TAIL           "${UTILS_BIN_DIR}/tail.exe")
    SET(PATH_ELF2MIF        "${UTILS_BIN_DIR}/elf2mif.exe")
    SET(PATH_ELFSTAT        "${UTILS_BIN_DIR}/elfstat.exe")
    SET(PATH_HEADERSTUFFER  "${UTILS_BIN_DIR}/headerstuffer.exe")
    SET(PATH_MIF2HEX        "${UTILS_BIN_DIR}/mif2hex.exe")
    SET(PATH_SYMFILT        "${UTILS_BIN_DIR}/symfilt.exe")
ELSE()
    SET(PATH_CAT            "cat")
    SET(PATH_MV             "mv")
    SET(PATH_HEAD           "head")
    SET(PATH_TAIL           "tail")
    SET(PATH_ELF2MIF        "${UTILS_BIN_DIR}/elf2mif")
    SET(PATH_ELFSTAT        "${UTILS_BIN_DIR}/elfstat")
    SET(PATH_HEADERSTUFFER  "${UTILS_BIN_DIR}/headerstuffer")
    SET(PATH_MIF2HEX        "${UTILS_BIN_DIR}/mif2hex")
    SET(PATH_SYMFILT        "${UTILS_BIN_DIR}/symfilt")
ENDIF()
