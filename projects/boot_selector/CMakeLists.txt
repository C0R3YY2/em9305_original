PROJECT(boot_selector C)


SET(${PROJECT_NAME}_SRCS
    source/boot_selector.c
    source/toolset.c
)

SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}

    # We want to have the uart_dma driver to send messages through
    # the uart line.
    uart_dma

    # We need the power management logic (PML) to access functions
    # used to change the next startup mode.
    pml

    # We need printf to be able to print messages in an easy way.
    printf

)

SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
)

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})
