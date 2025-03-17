# Add some global C flags (visibility: current folder and all subfolders).
MACRO(ADD_C_FLAGS)
    FOREACH(flag ${ARGN})
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
        SET(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} ${flag}")
    ENDFOREACH()
ENDMACRO(ADD_C_FLAGS)

# Remove some global C flags (visibility: current folder and all subfolders).
MACRO(REMOVE_C_FLAGS)
    FOREACH(flag ${ARGN})
        STRING(REPLACE "${flag}" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
        STRING(REPLACE "${flag}" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
        STRING(REPLACE "${flag}" "" CMAKE_C_LINK_FLAGS ${CMAKE_C_LINK_FLAGS})
    ENDFOREACH()

    STRING(REPLACE "  " " " CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    STRING(REPLACE "  " " " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    STRING(REPLACE "  " " " CMAKE_C_LINK_FLAGS ${CMAKE_C_LINK_FLAGS})

    STRING(STRIP ${CMAKE_C_FLAGS} CMAKE_C_FLAGS)
    STRING(STRIP ${CMAKE_CXX_FLAGS} CMAKE_CXX_FLAGS)
    STRING(STRIP ${CMAKE_C_LINK_FLAGS} CMAKE_C_LINK_FLAGS)
ENDMACRO(REMOVE_C_FLAGS)

# Add some global ASM flags (visibility: current folder and all subfolders).
MACRO(ADD_ASM_FLAGS)
    FOREACH(flag ${ARGN})
        SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${flag}")
    ENDFOREACH()
ENDMACRO(ADD_ASM_FLAGS)

# Remove some global ASM flags (visibility: current folder and all subfolders).
MACRO(REMOVE_ASM_FLAGS)
    FOREACH(flag ${ARGN})
        STRING(REPLACE "${flag}" "" CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS})
    ENDFOREACH()

    STRING(REPLACE "  " " " CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS})

    STRING(STRIP ${CMAKE_ASM_FLAGS} CMAKE_ASM_FLAGS)
ENDMACRO(REMOVE_ASM_FLAGS)

# Add some global LINK flags (visibility: current folder and all subfolders).
MACRO(ADD_LD_FLAGS)
    FOREACH(flag ${ARGN})
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flag}")
    ENDFOREACH()
ENDMACRO(ADD_LD_FLAGS)

# Remove some global LINK flags (visibility: current folder and all subfolders).
MACRO(REMOVE_LD_FLAGS)
    FOREACH(flag ${ARGN})
        STRING(REPLACE "${flag}" "" CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
    ENDFOREACH()

    STRING(REPLACE "  " " " CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

    STRING(STRIP ${CMAKE_EXE_LINKER_FLAGS} CMAKE_EXE_LINKER_FLAGS)
ENDMACRO(REMOVE_LD_FLAGS)

# Add some COMPILE flags to a specific target.
MACRO(ADD_TARGET_COMPILE_FLAGS target)
    FOREACH(flag ${ARGN})
        # Get the current compile flags and check if there is any.
        GET_TARGET_PROPERTY(compile_flags ${target} COMPILE_FLAGS)
        IF("${compile_flags}" STREQUAL "compile_flags-NOTFOUND")
            # Apply direcly the given compile flags.
            SET_TARGET_PROPERTIES(${target} PROPERTIES COMPILE_FLAGS "${flag}")
        ELSE()
            # Concatenate the previous compile flags with the given compile flags.
            SET_TARGET_PROPERTIES(${target} PROPERTIES COMPILE_FLAGS "${compile_flags} ${flag}")
        ENDIF()
    ENDFOREACH()
ENDMACRO(ADD_TARGET_COMPILE_FLAGS)

# Add some COMPILE flags to a target in all the build configurations.
MACRO(ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS target)
    FOREACH(build ${BUILD_CONFIGS})
        # Get the build target parameters.
        ARC_GET_BUILD_TARGET(${build} ${target})

        # Be sure the target exists.
        IF(TARGET ${build_target})
            # Add the COMPILE flags to the specific target.
            ADD_TARGET_COMPILE_FLAGS(${build_target} ${ARGN})
        ENDIF()

        # Try as well using the HW Design Iteration postfix.
        FOREACH(hw_di_item ${HW_DI_LIST})
            # Be sure the target exists.
            IF(TARGET ${build_target}_${hw_di_item})
                # Add the COMPILE flags to the specific target.
                ADD_TARGET_COMPILE_FLAGS(${build_target}_${hw_di_item} ${ARGN})
            ENDIF()
        ENDFOREACH()
    ENDFOREACH()
ENDMACRO(ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS)

# Add Compile options to a target in all build configurations. Required to pass EMB configuration includes
MACRO(ADD_TARGET_COMPILE_OPTIONS_ALL_BUILD_CONFIGS target)
    FOREACH(build ${BUILD_CONFIGS})
        # Get the build target parameters.
        ARC_GET_BUILD_TARGET(${build} ${target})

        # Be sure the target exists.
        IF(TARGET ${build_target})
            # Add the COMPILE flags to the specific target.
            TARGET_COMPILE_OPTIONS(${build_target} ${ARGN})
        ENDIF()

        # Try as well using the HW Design Iteration postfix.
        FOREACH(hw_di_item ${HW_DI_LIST})
            # Be sure the target exists.
            IF(TARGET ${build_target}_${hw_di_item})
                # Add the COMPILE flags to the specific target.
                TARGET_COMPILE_OPTIONS(${build_target}_${hw_di_item} ${ARGN})
            ENDIF()
        ENDFOREACH()
    ENDFOREACH()
ENDMACRO(ADD_TARGET_COMPILE_OPTIONS_ALL_BUILD_CONFIGS)

# Add some LINK flags to a specific target.
MACRO(ADD_TARGET_LD_FLAGS target)
    FOREACH(flag ${ARGN})
        # Get the current link flags and check if there is any.
        GET_TARGET_PROPERTY(link_flags ${target} LINK_FLAGS)
        IF("${link_flags}" STREQUAL "link_flags-NOTFOUND")
            # Apply direcly the given link flags.
            SET_TARGET_PROPERTIES(${target} PROPERTIES LINK_FLAGS "${flag}")
        ELSE()
            # Concatenate the previous link flags with the given link flags.
            SET_TARGET_PROPERTIES(${target} PROPERTIES LINK_FLAGS "${link_flags} ${flag}")
        ENDIF()
    ENDFOREACH()
ENDMACRO(ADD_TARGET_LD_FLAGS)

# Add some LINK flags to a target in all the build configurations.
MACRO(ADD_TARGET_LD_FLAGS_ALL_BUILD_CONFIGS target)
    FOREACH(build ${BUILD_CONFIGS})
        # Get the build target parameters.
        ARC_GET_BUILD_TARGET(${build} ${target})

        # Be sure the target exists.
        IF(TARGET ${build_target})
            # Add the LINK flags to the specific target.
            ADD_TARGET_LD_FLAGS(${build_target} ${ARGN})
        ENDIF()

        # Try as well using the HW Design Iteration postfix.
        FOREACH(hw_di_item ${HW_DI_LIST})
            # Be sure the target exists.
            IF(TARGET ${build_target}_${hw_di_item})
                # Add the LINK flags to the specific target.
                ADD_TARGET_LD_FLAGS(${build_target}_${hw_di_item} ${ARGN})
            ENDIF()
        ENDFOREACH()
    ENDFOREACH()
ENDMACRO(ADD_TARGET_LD_FLAGS_ALL_BUILD_CONFIGS)
