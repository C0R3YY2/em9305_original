PROJECT(emrscps_profile_server C)

# Library source files.
SET(${PROJECT_NAME}_SRCS
    source/em_rscps.c
)

# Library includes.
SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    ${EMB_COMMON_INCLUDES}
    includes
    ${LIBS_DIR}/emb_vs/rscps/profile/server/includes
)

LIB_IN_NVM()
ARC_ADD_LIBRARY(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})
ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${PROJECT_NAME} "-DNO_FLINT_ERRORS")

