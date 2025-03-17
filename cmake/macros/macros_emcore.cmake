# Macros for EM-Core handling

# Returns emcore_flavor as well as emcore_flavor_id.
# The format of the input is as follow: <emcore_flavor>:<emcore_flavor_id>
FUNCTION(GET_EMCORE_FLAVOR_INFO emcore_flavor_tuple emcore_flavor emcore_flavor_id)
    STRING(REPLACE ":" ";" emcore_flavor_list ${emcore_flavor_tuple})
    LIST(GET emcore_flavor_list 0 emcore_flavor_tmp)
    LIST(GET emcore_flavor_list 1 emcore_flavor_id_tmp)
    SET(${emcore_flavor} ${emcore_flavor_tmp} PARENT_SCOPE)
    SET(${emcore_flavor_id} ${emcore_flavor_id_tmp} PARENT_SCOPE)
    RETURN(PROPAGATE ${emcore_flavor} ${emcore_flavor_id})
ENDFUNCTION()


# Returns emcore_flavor_id of a given emcore_flavor.
# The format of the input is as follow: <emcore_flavor>
FUNCTION(GET_EMCORE_FLAVOR_ID emcore_flavor_name emcore_flavor_id is_custom_emcore)
    SET(emcore_flavor_id_internal -1)
    SET(${is_custom_emcore} 0 PARENT_SCOPE)
    IF(NOT ${emcore_flavor_name} STREQUAL "n/a")
        FOREACH(emcf ${EMCORE_FLAVORS})
            STRING(REPLACE ":" ";" emcore_flavor_list "${emcf}")
            LIST(GET emcore_flavor_list 0 emcore_flavor_name_tmp)
            LIST(GET emcore_flavor_list 1 emcore_flavor_id_tmp)
            IF(${emcore_flavor_name_tmp} STREQUAL ${emcore_flavor_name})
                SET(emcore_flavor_id_internal ${emcore_flavor_id_tmp})
                BREAK()
            ENDIF()
        ENDFOREACH()
        if (${emcore_flavor_id_internal} EQUAL -1)
            FOREACH(emcf ${CUSTOM_EMCORE_FLAVORS})
                STRING(REPLACE ":" ";" emcore_flavor_list "${emcf}")
                LIST(GET emcore_flavor_list 0 emcore_flavor_name_tmp)
                LIST(GET emcore_flavor_list 1 emcore_flavor_id_tmp)
                IF(${emcore_flavor_name_tmp} STREQUAL ${emcore_flavor_name})
                    SET(emcore_flavor_id_internal ${emcore_flavor_id_tmp})
                    SET(${is_custom_emcore} 1 PARENT_SCOPE)
                    BREAK()
                ENDIF()
            ENDFOREACH()
        ENDIF()
    ENDIF()
    SET(${emcore_flavor_id} ${emcore_flavor_id_internal} PARENT_SCOPE)

    # MESSAGE("ID ${emcore_flavor_id_internal} found for EMCore Flavor ${emcore_flavor_name}")
    RETURN(PROPAGATE ${emcore_flavor_id} ${is_custom_emcore})
ENDFUNCTION()
