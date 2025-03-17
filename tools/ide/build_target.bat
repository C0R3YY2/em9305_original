@echo off


if "%~1" == "[exe]" (

    rem Program the DVK
    cmake --build build --target %2

) else (

    echo Please select a target before building.

)
