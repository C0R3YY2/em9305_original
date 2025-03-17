@echo off

set BUILD_ROOT=".\build"

if exist %BUILD_ROOT% rmdir /S /Q %BUILD_ROOT%
mkdir %BUILD_ROOT%
cd %BUILD_ROOT%

echo Initialize CMake...
cmake -G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=8.5 -Wno-dev -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="cmake/toolchains/arcv2em.cmake" ..

rem Move projects files (.project and .cproject) into root directory
cmake -E rename .\.project ..\.project
cmake -E rename .\.cproject ..\.cproject

cd ..
