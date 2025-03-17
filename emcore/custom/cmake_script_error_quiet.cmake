EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} ARGS -P ${script}
    ERROR_VARIABLE error RESULT_VARIABLE res)
IF(NOT "${res}" STREQUAL "0")
    MESSAGE(">> Ignored error:\n====================\n${error}\n====================\n")
ENDIF()
