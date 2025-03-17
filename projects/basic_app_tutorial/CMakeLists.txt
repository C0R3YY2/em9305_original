
##
## CMakeLists.txt
##
## The CMake tool can be invoked with the following parameters:
##
## cmake --build . --target <target>
##
## Verbosity can be enforced with the `-v` option
## Cleaning first the build folder can be enforced with the `--clean-first` option.
##
## cmake --build . --target <target> -v --clean-first
##
## Target example:
##
## cmake --build . --target basic_app_tutorial -v --clean-first
##

PROJECT(basic_app_tutorial C)

# Search for all C files in src folder.
SET(${PROJECT_NAME}_SRCS
    nvm_main.c
    my_app/source/my_app.c
    my_app/source/my_app_task.c
)

# Set the libraries to link the final app with.
SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}
    uart_dma
    gpio
    printf
    QPC
    unitimer
    adc
)

# Set the include path to header files provided by the SDK.
SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em
    my_app/includes
)

# The final application will be located in NVM.
APP_IN_NVM()

# Provide the list of C source files to be compiled.
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})

# Provide the list of libraries to link with to build the final application.
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})

ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})

