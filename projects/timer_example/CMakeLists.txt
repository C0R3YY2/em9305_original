PROJECT(timer_example C)


SET(${PROJECT_NAME}_SRCS_ONESHOT
    nvm_main_oneshot.c
)

SET(${PROJECT_NAME}_SRCS_EXPRESSO
    nvm_main_expresso.c
)

SET(${PROJECT_NAME}_SRCS_PERIODIC
    nvm_main_periodic.c
)

SET(${PROJECT_NAME}_LIBS_COMMON
    ${NVM_COMMON_LIBS}

    unitimer
)

SET(${PROJECT_NAME}_LIBS_PERIODIC
    ${${PROJECT_NAME}_LIBS_COMMON}

    QPC
)

SET(${PROJECT_NAME}_INCLUDES_COMMON
    ${COMMON_INCLUDES}
)

SET(${PROJECT_NAME}_INCLUDES_PERIODIC
    ${${PROJECT_NAME}_INCLUDES_COMMON}

    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em
)

# oneshot
APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME}_oneshot ${${PROJECT_NAME}_SRCS_ONESHOT})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_oneshot ${${PROJECT_NAME}_LIBS_COMMON})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_oneshot ${${PROJECT_NAME}_INCLUDES_COMMON})

# expresso

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME}_expresso ${${PROJECT_NAME}_SRCS_EXPRESSO})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_expresso ${${PROJECT_NAME}_LIBS_COMMON})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_expresso ${${PROJECT_NAME}_INCLUDES_COMMON})

# periodic

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME}_periodic ${${PROJECT_NAME}_SRCS_PERIODIC})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_periodic ${${PROJECT_NAME}_LIBS_PERIODIC})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_periodic ${${PROJECT_NAME}_INCLUDES_PERIODIC})
