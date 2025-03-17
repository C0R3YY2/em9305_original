# Add common compilation flags.
SET(EMB_COMMON_FLAGS
    -DNO_FLINT
    -Wno-implicit-int-conversion
    -Wno-sign-conversion
    -Wno-shorten-64-to-32
    -Wno-missing-braces
    -Wno-enum-conversion
)
ADD_C_FLAGS(${EMB_COMMON_FLAGS})
ADD_LD_FLAGS(${EMB_COMMON_FLAGS})

# Add define for EM_DERIVED_STACK_CODE
ADD_C_FLAGS(-DEM_DERIVED_STACK_CODE=1)
