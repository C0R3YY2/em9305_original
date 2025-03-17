PROJECT(fwu_service C)

# Library source files.
SET(${PROJECT_NAME}_SRCS
    source/fwu_svc.c
)

# Library includes.
SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    ${EMB_COMMON_INCLUDES}
    includes
)

# Library with full features.
LIB_IN_NVM()
ARC_ADD_LIBRARY(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})
ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${PROJECT_NAME} "-DNO_FLINT_ERRORS")

# Library with limited features to restart an application in FWU Mode.
# This version does not embed the FWU Data characteristics and most of the
# Control Point procedures are not permitted.
LIB_IN_NVM()
ARC_ADD_LIBRARY(${PROJECT_NAME}_light ${${PROJECT_NAME}_SRCS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_light ${${PROJECT_NAME}_INCLUDES})
ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${PROJECT_NAME}_light "-DNO_FLINT_ERRORS -DLIGHT_VARIANT")

